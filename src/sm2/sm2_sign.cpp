//
// Created by acherstyx on 8/16/20.
//

#include "sm2_sign.h"
#include <ctime>
#include <cstdlib>

//#define SM2_SIGN_DEBUG

Big hash_Za(unsigned char *ID_A, unsigned int ID_A_len, const Big &a, const Big &b,
            const Big &x_g, const Big &y_g, const Big &x_a, const Big &y_a) {
#ifdef SM2_SIGN_DEBUG
    cout << "ID: " << ID_A << " ID len: " << ID_A_len << "\n";
    cout << "a: " << a << " b: " << b << "\n";
    cout << "xa: " << x_a << "\n" << "ya: " << y_a << "\n";
#endif
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


#ifdef SM2_SIGN_DEBUG
    cout << "Z_A: " << char2big(hash_result, 32) << "\n";
#endif
    return char2big(hash_result, 32);
}


void sm2_sign(unsigned char *ID_A, unsigned int ID_A_len, unsigned char *message, unsigned int message_len,
              const Big &key, Big &r, Big &s) {
    miracl *mip = mirsys(20, 0);
    FPECC ecc_config = Ecc256;
    Big a, b, p, n, g_x, g_y;

    unsigned char *M;
    unsigned char Za_char[1000];
    unsigned int Za_len;
    unsigned char e_char[32]; // hash 256
    Big Za, e, k, x1, y1, xd, yd, z;
    epoint *g;

    // ecc parameter
    mip->IOBASE = 16;
    cinstr(p.getbig(), ecc_config.p);
    cinstr(a.getbig(), ecc_config.a);
    cinstr(b.getbig(), ecc_config.b);
    cinstr(n.getbig(), ecc_config.n);
    cinstr(g_x.getbig(), ecc_config.x);
    cinstr(g_y.getbig(), ecc_config.y);
    mip->IOBASE = 10;
    // init ecc
    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    g = epoint_init();
    epoint_set(g_x.getbig(), g_y.getbig(), 0, g);

    // Za
    epoint *pa = epoint_init();
    epoint_set(g_x.getbig(), g_y.getbig(), 0, pa);
    ecurve_mult(key.getbig(), pa, pa);
    Big xa, ya;
    epoint_get(pa, xa.getbig(), ya.getbig());
    Za = hash_Za(ID_A, ID_A_len, a, b, g_x, g_y, xa, ya);

    // Z_a || M
    big2char(Za, Za_char, Za_len);
    M = (unsigned char *) malloc(sizeof(unsigned char) * (message_len + Za_len));
    memcpy(M, Za_char, Za_len);
    memcpy(M + Za_len, message, message_len);
#ifdef SM2_SIGN_DEBUG
    cout << "Za len: " << Za_len << "\n";
#endif

    // hash(M)
    SM3Calc(M, message_len + Za_len, e_char);
    free(M);
    e = char2big(e_char, 32);
#ifdef SM2_SIGN_DEBUG
    cout << "e in sign: " << e << "\n";
#endif

    restart_sign:

    // random k
    struct timespec tn{};
    clock_gettime(CLOCK_REALTIME, &tn);
    irand(unsigned(tn.tv_nsec));
    bigrand(n.getbig(), k.getbig());

#ifdef SM2_SIGN_DEBUG
    cout << "rand k: " << k << "\n";
#endif

    // c1 -> <x1, y1>
    ecurve_mult(k.getbig(), g, g);
    epoint_get(g, x1.getbig(), y1.getbig());

    // r, s
    r = (e + x1) % n;
    if (r == 0 || r + k == n) goto restart_sign;
    xgcd((1 + key).getbig(), n.getbig(), xd.getbig(), yd.getbig(), z.getbig());
    if (z != 1) throw exception();  // check
    s = ((xd * (k - r * key)) % n + n) % n;
    if (s == 0) goto restart_sign;
}

bool sm2_verify(unsigned char *ID_A, unsigned int ID_A_len, unsigned char *message, unsigned int message_len,
                const Big &r, const Big &s, const Big &xa, const Big &ya) {
    miracl *mip = mirsys(20, 0);
    FPECC ecc_config = Ecc256;
    Big a, b, p, n, g_x, g_y;
    epoint *g, *pa;

    unsigned char *M;
    unsigned char e_char[32]; // hash 256
    Big Za, e, t, x1, y1, R;

    // ecc parameter
    mip->IOBASE = 16;
    cinstr(p.getbig(), ecc_config.p);
    cinstr(a.getbig(), ecc_config.a);
    cinstr(b.getbig(), ecc_config.b);
    cinstr(n.getbig(), ecc_config.n);
    cinstr(g_x.getbig(), ecc_config.x);
    cinstr(g_y.getbig(), ecc_config.y);
    mip->IOBASE = 10;
    // init ecc
    ecurve_init(a.getbig(), b.getbig(), p.getbig(), MR_PROJECTIVE);
    g = epoint_init();
    pa = epoint_init();
    epoint_set(g_x.getbig(), g_y.getbig(), 0, g);
    epoint_set(xa.getbig(), ya.getbig(), 0, pa);

    // Za
    epoint_get(pa, xa.getbig(), ya.getbig());
    Za = hash_Za(ID_A, ID_A_len, a, b, g_x, g_y, xa, ya);

    // check r, s
    if (r < 1 || r >= n) {
#ifdef SM2_SIGN_DEBUG
        cout << "Signature invalid: r < 1 or r >= n" << "\n";
#endif
        return false;
    }
    if (s < 1 || s >= n) {
#ifdef SM2_SIGN_DEBUG
        cout << "Signature invalid: s < 1 or s >= n" << "\n";
#endif
        return false;
    }

    // Za
    unsigned int Za_len;
    unsigned char Za_char[1000];
    big2char(Za, Za_char, Za_len);
    // Za || M
    M = (unsigned char *) malloc(sizeof(char) * (Za_len + message_len));
    memcpy(M, Za_char, Za_len);
    memcpy(M + Za_len, message, message_len);

    // e
    SM3Calc(M, (unsigned int) (Za_len + message_len), e_char);
    free(M);
    e = char2big(e_char, 32);
#ifdef SM2_SIGN_DEBUG
    cout << "e in verify: " << e << "\n";
#endif

    // t
    t = (r + s) % n;
#ifdef SM2_SIGN_DEBUG
    cout << "t: " << t << "\n";
#endif
    if (t == 0) {
#ifdef SM2_SIGN_DEBUG
        cout << "Signature invalid: t!=0" << "\n";
#endif
        return false;
    }

    ecurve_mult(s.getbig(), g, g);
    ecurve_mult(t.getbig(), pa, pa);
    ecurve_add(pa, g); // g = pa+g <- s*G+t*P_A

    epoint_get(g, x1.getbig(), y1.getbig());

    R = (e + x1) % n;
#ifdef SM2_SIGN_DEBUG
    cout << "R: " << R << "\n" << "r: " << r << "\n";
#endif
    return R == r ? true : false;
}
