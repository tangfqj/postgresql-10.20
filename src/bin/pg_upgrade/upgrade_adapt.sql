--
-- SQL queries for upgrade tests across different major versions.
--
-- This file includes a set of SQL queries to make a cluster to-be-upgraded
-- compatible with the version this file is based on.  Note that this
-- requires psql, as per-version queries are controlled with a set of \if
-- clauses.

-- This script is backward-compatible, so it is able to work with any version
-- newer than 9.2 we are upgrading from, up to the branch this script is stored
-- on (even if this would not run if running pg_upgrade with the same version
-- for the origin and the target).

-- \if accepts a simple boolean value, so all the version checks are
-- saved based on this assumption.
SELECT
  ver <= 902 AS oldpgversion_le92,
  ver <= 904 AS oldpgversion_le94,
  ver <= 906 AS oldpgversion_le96
  FROM (SELECT current_setting('server_version_num')::int / 100 AS ver) AS v;
\gset

-- Objects last appearing in 9.2.
\if :oldpgversion_le92
-- Note that those tables are removed from the regression tests in 9.3
-- and newer versions.
DROP TABLE abstime_tbl;
DROP TABLE reltime_tbl;
DROP TABLE tinterval_tbl;
\endif

-- Objects last appearing in 9.4.
\if :oldpgversion_le94
-- This aggregate has been fixed in 9.5 and later versions, so drop
-- and re-create it.
DROP AGGREGATE array_cat_accum(anyarray);
CREATE AGGREGATE array_larger_accum (anyarray) (
                  sfunc = array_larger,
                  stype = anyarray,
                  initcond = $${}$$);
-- This operator has been fixed in 9.5 and later versions, so drop and
-- re-create it.
DROP OPERATOR @#@ (NONE, bigint);
CREATE OPERATOR @#@ (PROCEDURE = factorial,
                     RIGHTARG = bigint);
\endif

-- Objects last appearing in 9.6.
\if :oldpgversion_le96
DROP FUNCTION public.oldstyle_length(integer, text);
\endif
