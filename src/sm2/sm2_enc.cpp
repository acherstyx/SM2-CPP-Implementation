//
// Created by acherstyx on 8/7/20.
//

#include <ctime>
#include <cstdlib>
#include "sm2_enc.h"
#include "big.h"
#include "ecn.h"

typedef struct FPECC {
    char *p;
    char *a;
    char *b;
    char *n;
    char *x;
    char *y;
} FPECC;

/*SM2*/
struct FPECC Ecc256 = {
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF",
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC",
        "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93",
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123",
        "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7",
        "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0",
};

void sm2_key_gen(Big &x, Big &y, Big &private_key) {
    Big a, b, p, n;
    miracl *mip = mirsys(20, 0);
    epoint *g;

    mip->IOBASE = 16;

    // use static ecc parameter
    cinstr(p.getbig(), Ecc256.p);
    cinstr(a.getbig(), Ecc256.a);
    cinstr(b.getbig(), Ecc256.b);
    cinstr(n.getbig(), Ecc256.n);
    cinstr(x.getbig(), Ecc256.x);
    cinstr(y.getbig(), Ecc256.y);

    // define ecc
    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    g = epoint_init();
    epoint_set(x.getbig(), y.getbig(), 0, g);
    // rand private key
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    irand(unsigned(tn.tv_nsec));

    bigrand(n.getbig(), private_key.getbig());
    // point
    ecurve_mult(private_key.getbig(), g, g);
    epoint_get(g, x.getbig(), y.getbig());
    mip->IOBASE = 10;
}

int kdf(unsigned char *zl, unsigned char *zr, int klen, unsigned char *kbuf) {
/*
return 0: kbuf is 0, unusable
       1: kbuf is OK
*/
    unsigned char buf[70];
    unsigned char digest[32];
    unsigned int ct = 0x00000001;
    int i, m, n;
    unsigned char *p;


    memcpy(buf, zl, 32);
    memcpy(buf + 32, zr, 32);

    m = klen / 32;
    n = klen % 32;
    p = kbuf;

    for (i = 0; i < m; i++) {
        buf[64] = (ct >> 24) & 0xFF;
        buf[65] = (ct >> 16) & 0xFF;
        buf[66] = (ct >> 8) & 0xFF;
        buf[67] = ct & 0xFF;
        SM3Calc(buf, 68, p);
        p += 32;
        ct++;
    }

    if (n != 0) {
        buf[64] = (ct >> 24) & 0xFF;
        buf[65] = (ct >> 16) & 0xFF;
        buf[66] = (ct >> 8) & 0xFF;
        buf[67] = ct & 0xFF;
        SM3Calc(buf, 68, digest);
    }

    memcpy(p, digest, n);

    for (i = 0; i < klen; i++) {
        if (kbuf[i] != 0)
            break;
    }

    if (i < klen)
        return 1;
    else
        return 0;
}

int sm2_enc(unsigned char *msg, int msg_len, Big x, Big y, unsigned char *msg_after_enc) {
    Big a, b, p, n;
    Big g_x, g_y;   // g = <g_x, g_y>
    Big k;  // random number for encrypt
    FPECC ecc_config = Ecc256; // default ecc
    miracl *mip = mirsys(20, 0);
    epoint *g, *pb; // public key
    Big x1, y1;
    Big x2, y2;
    unsigned char zl[32], zr[32];

    mip->IOBASE = 16;

    // get ecc parameter
    cinstr(p.getbig(), ecc_config.p);
    cinstr(a.getbig(), ecc_config.a);
    cinstr(b.getbig(), ecc_config.b);
    cinstr(n.getbig(), ecc_config.n);

    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    // read g
    cinstr(g_x.getbig(), ecc_config.x);
    cinstr(g_y.getbig(), ecc_config.y);
    g = epoint_init();

    epoint_set(g_x.getbig(), g_y.getbig(), 0, g);

    // read pb
    pb = epoint_init();

    epoint_set(x.getbig(), y.getbig(), 0, pb);

    // random k
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    irand(unsigned(tn.tv_nsec));
    bigrand(n.getbig(), k.getbig());
//    cout << "rand k: " << k << "\n";

    // c1
    ecurve_mult(k.getbig(), g, g);
    epoint_get(g, x1.getbig(), y1.getbig());
    big_to_bytes(32, x1.getbig(), (char *) msg_after_enc, TRUE);
    big_to_bytes(32, y1.getbig(), (char *) msg_after_enc + 32, TRUE);

//    cout << "C1: " << c1_x;
//    cout << " " << c1_y << "\n";

    // c2
    ecurve_mult(k.getbig(), pb, pb);
    epoint_get(pb, x2.getbig(), y2.getbig());

//    cout << "[enc] x2: " << c2_x << " y2: " << c2_y << '\n';

    big_to_bytes(32, x2.getbig(), (char *) zl, TRUE);
    big_to_bytes(32, y2.getbig(), (char *) zr, TRUE);

    kdf(zl, zr, msg_len, msg_after_enc + 64);

    for (int i = 0; i < msg_len; i++) {
        msg_after_enc[i + 64] ^= msg[i];
    }

    unsigned char *temp = (unsigned char *) malloc(sizeof(unsigned char) * (32 + 32 + msg_len));
    memcpy(temp, zl, 32);
    memcpy(temp + msg_len, msg, msg_len);
    memcpy(temp + msg_len + 32, zr, msg_len);
    SM3Calc(temp, 63 + msg_len, msg_after_enc + 64 + msg_len);

    mip->IOBASE = 10;
    return msg_len + 96;
}

int sm2_dec(unsigned char *msg, int msg_len, Big d, unsigned char *msg_dec) {
    int klen = msg_len - 96;
    Big a, b, p, n;
    epoint *pb;
    FPECC ecc_config = Ecc256; // default ecc
    miracl *mip = mirsys(20, 0);
    Big x1, y1; // c1
    Big x2, y2;
    unsigned char zl[32], zr[32];

    mip->IOBASE = 16;

    // get ecc parameter
    cinstr(p.getbig(), ecc_config.p);
    cinstr(a.getbig(), ecc_config.a);
    cinstr(b.getbig(), ecc_config.b);
    cinstr(n.getbig(), ecc_config.n);

    bytes_to_big(32, (char *) msg, x1.getbig());
    bytes_to_big(32, (char *) msg + 32, y1.getbig());

    // t
    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    pb = epoint_init();
    epoint_set(x1.getbig(), y1.getbig(), 0, pb);
    ecurve_mult(d.getbig(), pb, pb);
    epoint_get(pb, x2.getbig(), y2.getbig());
    big_to_bytes(32, x2.getbig(), (char *) zl, TRUE);
    big_to_bytes(32, y2.getbig(), (char *) zr, TRUE);

//    cout << "[dec] x2: " << x2 << " y2: " << y2 << '\n';

    kdf(zl, zr, klen, msg_dec);

    for (int i = 0; i < klen; i++) {
        msg_dec[i] ^= msg[i + 64];
    }


    mip->IOBASE = 10;

    return klen;
}