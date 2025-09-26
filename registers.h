
/*

  flags register:
  | N | V | 1 | B | D | I | Z | C |
  ^MSB                         LSB^

  N = negative (1 = neg)
  V = Overflow (1 = true)
  1 = undef.
  B = BRK command (1 = BRK, 0 = IRQB)
  D = Decimal mode (1 = true)
  I = IRQB disable (1 = disable)
  Z = zero (1 = true)
  C = carry (1 = true)

  source: https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf
 */

