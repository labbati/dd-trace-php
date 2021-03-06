--TEST--
hook_method posthook is called with the correct scope (no override)
--FILE--
<?php

DDTrace\hook_method('BaseClass', 'speak',
    null,
    function ($This, $scope, $args) {
        echo "${scope}::speak hooked.\n";
    }
);

DDTrace\hook_method('ChildClass', 'speak',
    null,
    function ($This, $scope, $args) {
        echo "${scope}::speak hooked.\n";
    }
);

class BaseClass
{
    public static function speak()
    {
        echo "I have spoken.\n";
    }
}

final class ChildClass extends BaseClass
{
}

final class Orthogonal
{
    public static function run()
    {
        BaseClass::speak();
        ChildClass::speak();
    }
}

Orthogonal::run();
?>
--EXPECT--
I have spoken.
BaseClass::speak hooked.
I have spoken.
ChildClass::speak hooked.
