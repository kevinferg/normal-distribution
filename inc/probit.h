#ifndef PROBIT_H
#define PROBIT_H

/*******************************************************************
Algorithm AS 241: The Percentage Points of the Normal Distribution
Author: Michael J. Wichura
Source: Journal of the Royal Statistical Society. Series C (Applied
        Statistics), 1988, Vol. 37, No. 3 (1988), pp. 477-484
Published by: Wiley for the Royal Statistical Society
Stable URL: https://www.jstor.org/stabl/2347330
*********************************************************************/

double probit7(double p);
double probit16(double p);

/*********** Test functions ************/
int test_probit(void);

#endif

/************* Test function output ***********

 ---- Coefficient hash checks ----

> Hash AB_PREC7
  Calculated: 32.3184577772
    Expected: 32.3184577772
  Match? Yes

> Hash CD_PREC7
  Calculated: 15.7614929821
    Expected: 15.7614929821
  Match? Yes

> Hash EF_PREC7
  Calculated: 19.4052910204
    Expected: 19.4052910204
  Match? Yes

probit(2.50e-001) = ...
  Calculated,  7: -0.6744897742392232
      True Value: -0.6744897501960817

probit(1.00e-003) = ...
  Calculated,  7: -3.0902319494250929
      True Value: -3.0902323061678141

probit(1.00e-020) = ...
  Calculated,  7: -9.2623407449740274
      True Value: -9.2623400897984087

************************************************/