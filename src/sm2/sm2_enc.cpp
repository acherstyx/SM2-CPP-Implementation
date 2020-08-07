//
// Created by acherstyx on 8/7/20.
//

#include "sm2_enc.h"
#include "big.h"
#include "ecn.h"

//Miracl precision(2048, 2);

struct FPECC {
    char *p;
    char *a;
    char *b;
    char *n;
    char *x;
    char *y;
};

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
    irand(time(NULL));
    bigrand(n.getbig(), private_key.getbig());
    // point
    ecurve_mult(private_key.getbig(), g, g);
    epoint_get(g, x.getbig(), y.getbig());
}

