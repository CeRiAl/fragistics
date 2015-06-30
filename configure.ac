AC_INIT([fragistics], [1.6.0])
AC_CONFIG_SRCDIR([src/gameslogparser.cpp])
AM_INIT_AUTOMAKE

dnl programs
AC_PATH_PROG(PERL,perl)
AC_PATH_PROG(GREP,grep)
AC_PROG_AWK
AC_PROG_CXX

AC_SUBST(PKGLIBDIR)

AC_OUTPUT(
  Makefile
  src/Makefile
  conf/Makefile
  logs/Makefile
  perl/Makefile
  tools/Makefile
  images/Makefile
  images/chart/Makefile
  images/models/Makefile
  images/weapons/Makefile
  images/levelshots/Makefile
  templates/Makefile
)
