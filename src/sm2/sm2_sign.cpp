//
// Created by acherstyx on 8/16/20.
//

#include "sm2_sign.h"
#include <ctime>
#include <cstdlib>

Big hash_Za(unsigned char *ID_A, unsigned int ID_A_len, const Big &a, const Big &b,
            const Big &x_g, const Big &y_g, const Big &x_a, const Big &y_a) {
    unsigned char ENTL[2], a_char[1000], b_char[1000], x_g_char[1000], y_g_char[1000], x_a_char[1000], y_a_char[1000];
    unsigned int ENTL_len, a_len, b_len, x_g_len, y_g_len, x_a_len, y_a_len;

    unsigned char message[6002];
    unsigned char hash_result[32];

    big2char(Big(ID_A_len % 0xffff), ENTL, ENTL_len);
    if (ENTL_len > 2)
        throw exception();

    big2char(a, a_char, a_len);
    big2char(b, b_char, b_len);
    big2char(x_g, x_g_char, x_g_len);
    big2char(y_g, y_g_char, y_g_len);
    big2char(x_a, x_a_char, x_a_len);
    big2char(y_a, y_a_char, y_a_len);

    memcpy(message, ENTL, ENTL_len);
    memcpy(message + ENTL_len, ID_A, ID_A_len);
    memcpy(message + ENTL_len + ID_A_len, a_char, a_len);
    memcpy(message + ENTL_len + ID_A_len + a_len, b_char, b_len);
    memcpy(message + ENTL_len + ID_A_len + a_len + b_len, x_g_char, x_g_len);
    memcpy(message + ENTL_len + ID_A_len + a_len + b_len + x_g_len, y_g_char, y_g_len);
    memcpy(message + ENTL_len + ID_A_len + a_len + b_len + x_g_len + y_g_len, x_a_char, x_a_len);
    memcpy(message + ENTL_len + ID_A_len + a_len + b_len + x_g_len + y_g_len + x_a_len, y_a_char, y_a_len);

    SM3Calc(message, ENTL_len + ID_A_len + a_len + b_len + x_g_len + y_g_len + x_a_len + y_a_len, hash_result);

    return char2big(hash_result, 32);
}


void sm2_sign(const Big &Za, unsigned char *message, unsigned int message_len, const Big &key, Big &r, Big &s) {
    miracl *mip = mirsys(20, 0);
    FPECC ecc_config = Ecc256;
    Big a, b, p, n, g_x, g_y;

    unsigned char *M;
    unsigned char Za_char[1000];
    unsigned int Za_len;
    unsigned char e_char[32]; // hash 256
    Big e, k, x1, y1, xd, yd, z;
    epoint *g;


    // ecc parameter
    mip->IOBASE = 16;
    cinstr(p.getbig(), ecc_config.p);
    cinstr(a.getbig(), ecc_config.a);
    cinstr(b.getbig(), ecc_config.b);
    cinstr(n.getbig(), ecc_config.n);
    cinstr(g_x.getbig(), ecc_config.x);
    cinstr(g_y.getbig(), ecc_config.y);
    // init ecc
    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    g = epoint_init();
    epoint_set(g_x.getbig(), g_y.getbig(), 0, g);

    // Z_a || M
    big2char(Za, Za_char, Za_len);
    M = (unsigned char *) malloc(sizeof(unsigned char) * (message_len + Za_len));
    memcpy(M, Za_char, Za_len);
    memcpy(M + Za_len, message, message_len);

    // hash(M)
    SM3Calc(M, message_len + Za_len, e_char);
    e = char2big(e_char, 32);

    restart_sign:

    // random k
    struct timespec tn{};
    clock_gettime(CLOCK_REALTIME, &tn);
    irand(unsigned(tn.tv_nsec));
    bigrand(n.getbig(), k.getbig());

#ifdef DEBUG
    cout << "[enc] rand k: " << k << "\n";
#endif

    // c1 -> <x1, y1>
    ecurve_mult(k.getbig(), g, g);
    epoint_get(g, x1.getbig(), y1.getbig());

    // r, s
    r = (e + x1) % n;
    if (r == 0 || r + k == n) goto restart_sign;
    xgcd((1 + key).getbig(), p.getbig(), xd.getbig(), yd.getbig(), z.getbig());
    if (z != 1) throw exception();  // check
    s = (xd * (k - r * key)) % n;
    if (s == 0) goto restart_sign;
}

