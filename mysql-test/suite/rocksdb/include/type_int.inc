#
# INT column types
#

# $extra_col_opts is set by a calling test
# $extra_col_opts_ddl_err is set by a calling test
# $extra_col_opts_dml_err is set by a calling test

eval CREATE TABLE t1 (
  i INT $extra_col_opts,
  i0 INT(0) $extra_col_opts,
  i1 INT(1) $extra_col_opts,
  i20 INT(20) $extra_col_opts,
  t TINYINT $extra_col_opts,
  t0 TINYINT(0) $extra_col_opts,
  t1 TINYINT(1) $extra_col_opts,
  t20 TINYINT(20) $extra_col_opts,
  s SMALLINT $extra_col_opts,
  s0 SMALLINT(0) $extra_col_opts,
  s1 SMALLINT(1) $extra_col_opts,
  s20 SMALLINT(20) $extra_col_opts,
  m MEDIUMINT $extra_col_opts,
  m0 MEDIUMINT(0) $extra_col_opts,
  m1 MEDIUMINT(1) $extra_col_opts,
  m20 MEDIUMINT(20) $extra_col_opts,
  b BIGINT $extra_col_opts,
  b0 BIGINT(0) $extra_col_opts,
  b1 BIGINT(1) $extra_col_opts,
  b20 BIGINT(20) $extra_col_opts,
  pk INT AUTO_INCREMENT PRIMARY KEY
) ENGINE=rocksdb;
SHOW COLUMNS IN t1;

# Always valid values

INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (2147483647,2147483647,2147483647,2147483647,127,127,127,127,32767,32767,32767,32767,8388607,8388607,8388607,8388607,9223372036854775807,9223372036854775807,9223372036854775807,9223372036854775807);
--sorted_result
SELECT i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20 FROM t1;

# Values which can be valid or not,
# depending on whether columns are SIGNED or UNSIGNED
# (if not valid should produce warnings)

if ($extra_col_opts_dml_err == 0)
{
  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (-2147483648,-2147483648,-2147483648,-2147483648,-128,-128,-128,-128,-32768,-32768,-32768,-32768,-8388608,-8388608,-8388608,-8388608,-9223372036854775808,-9223372036854775808,-9223372036854775808,-9223372036854775808);

  # 5.7 uses strict mode by default, so this will now return an error
  --error ER_WARN_DATA_OUT_OF_RANGE
  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (4294967295,4294967295,4294967295,4294967295,255,255,255,255,65535,65535,65535,65535,16777215,16777215,16777215,16777215,18446744073709551615,18446744073709551615,18446744073709551615,18446744073709551615);

  # now test with the 5.6 behavior just to be sure
  set @orig_sql_mode=@@session.sql_mode;
  set session sql_mode="";
  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (4294967295,4294967295,4294967295,4294967295,255,255,255,255,65535,65535,65535,65535,16777215,16777215,16777215,16777215,18446744073709551615,18446744073709551615,18446744073709551615,18446744073709551615);
  set session sql_mode=@orig_sql_mode;
}
if ($extra_col_opts_dml_err == 1)
{
  # 5.7 uses strict mode by default, so this will now return an error
  --error ER_WARN_DATA_OUT_OF_RANGE
  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (-2147483648,-2147483648,-2147483648,-2147483648,-128,-128,-128,-128,-32768,-32768,-32768,-32768,-8388608,-8388608,-8388608,-8388608,-9223372036854775808,-9223372036854775808,-9223372036854775808,-9223372036854775808);

  # now test with the 5.6 behavior just to be sure
  set @orig_sql_mode=@@session.sql_mode;
  set session sql_mode="";
  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (-2147483648,-2147483648,-2147483648,-2147483648,-128,-128,-128,-128,-32768,-32768,-32768,-32768,-8388608,-8388608,-8388608,-8388608,-9223372036854775808,-9223372036854775808,-9223372036854775808,-9223372036854775808);
  set session sql_mode=@orig_sql_mode;

  INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (4294967295,4294967295,4294967295,4294967295,255,255,255,255,65535,65535,65535,65535,16777215,16777215,16777215,16777215,18446744073709551615,18446744073709551615,18446744073709551615,18446744073709551615);
}

--sorted_result
SELECT i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20 FROM t1;

# Invalid values

# 5.7 uses strict mode by default, so this will now return an error
--error ER_WARN_DATA_OUT_OF_RANGE
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (-2147483649,-2147483649,-2147483649,-2147483649,-129,-129,-129,-129,-32769,-32769,-32769,-32769,-8388609,-8388609,-8388609,-8388609,-9223372036854775809,-9223372036854775809,-9223372036854775809,-9223372036854775809);

# now test with the 5.6 behavior just to be sure
set @orig_sql_mode=@@session.sql_mode;
set session sql_mode="";
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (-2147483649,-2147483649,-2147483649,-2147483649,-129,-129,-129,-129,-32769,-32769,-32769,-32769,-8388609,-8388609,-8388609,-8388609,-9223372036854775809,-9223372036854775809,-9223372036854775809,-9223372036854775809);
set session sql_mode=@orig_sql_mode;

# 5.7 uses strict mode by default, so this will now return an error
--error ER_WARN_DATA_OUT_OF_RANGE
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (4294967296,4294967296,4294967296,4294967296,256,256,256,256,65536,65536,65536,65536,16777216,16777216,16777216,16777216,18446744073709551616,18446744073709551616,18446744073709551616,18446744073709551616);

# now test with the 5.6 behavior just to be sure
set @orig_sql_mode=@@session.sql_mode;
set session sql_mode="";
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) VALUES (4294967296,4294967296,4294967296,4294967296,256,256,256,256,65536,65536,65536,65536,16777216,16777216,16777216,16777216,18446744073709551616,18446744073709551616,18446744073709551616,18446744073709551616);
set session sql_mode=@orig_sql_mode;

set @orig_tx_iso=@@session.tx_isolation;
set session tx_isolation='READ-COMMITTED';
# 5.7 uses strict mode by default, so this will now return an error
--error ER_WARN_DATA_OUT_OF_RANGE
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) SELECT b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b FROM t1 WHERE b IN (-9223372036854775808,9223372036854775807,18446744073709551615);

# now test with the 5.6 behavior just to be sure
set @orig_sql_mode=@@session.sql_mode;
set session sql_mode="";
INSERT INTO t1 (i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20) SELECT b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b FROM t1 WHERE b IN (-9223372036854775808,9223372036854775807,18446744073709551615);
set session sql_mode=@orig_sql_mode;
set session tx_isolation=@orig_tx_iso;

--sorted_result
SELECT i,i0,i1,i20,t,t0,t1,t20,s,s0,s1,s20,m,m0,m1,m20,b,b0,b1,b20 FROM t1;

--error ER_TOO_BIG_DISPLAYWIDTH
eval ALTER TABLE t1 ADD COLUMN i257 INT(257) $extra_col_opts;

DROP TABLE t1;