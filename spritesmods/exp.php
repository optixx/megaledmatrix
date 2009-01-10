#!/usr/bin/php
<?

$base=2;
echo "unsigned char exptab[] PROGMEM={";
for ($x=0; $x<32; $x++) {
    echo intval((pow($x,$base)/pow(31,$base))*255).",";
}
echo "};\n";
?>