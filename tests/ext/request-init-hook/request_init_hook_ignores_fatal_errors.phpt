--TEST--
Request init hook ignores fatal errors
--SKIPIF--
<?php if (PHP_VERSION_ID < 50600) die('skip: Cannot recover from fatal errors until PHP 5.6'); ?>
--INI--
ddtrace.request_init_hook={PWD}/raises_fatal_error.php
--FILE--
<?php
echo "Request start" . PHP_EOL;

?>
--EXPECT--
Calling a function that does not exist...
Request start
