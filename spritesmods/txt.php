#!/usr/bin/php
<?
//Simple program to send a text to the ledboards

system("stty 500000 < /dev/ttyUSB0");
$str="Some text to dump on the boards. Yadayada.";

//board positions
$boff=array(
    array(2,0),
    array(2,1),
    array(2,2),
    array(1,2),
    array(1,1),
    array(1,0),
    array(0,0),
    array(0,1),
    array(0,2));

$im=imagecreatetruecolor(96,48);
$bg=imagecolorallocate($im,0,0,0);
$fg=imagecolorallocate($im,255,0,0);

$com=fopen("/dev/ttyUSB0","w");
//$com=fopen("/tmp/bla","w");
echo("Open.\n");
while(1) {
    $x=0; $y=0;
    for ($l=0; $l<strlen($str); $l++) {
	$c=substr($str,$l,1);
	imagestring($im,2,$x,$y,ord($c)<32?" ":$c,$fg);
	$x+=6;
	if ($c=="\n" || $x>96) {
	    $x=0;
	    $y+=9;
	}
    }
    $data=chr(0x80);
    $data.=chr(0x80);
    $data.=chr(0x80);
    $data.=chr(0x80);
    $data.=chr(0x80);
    for ($brd=0; $brd<9; $brd++) {
	$xx=$boff[$brd][0]*32;
	$yy=$boff[$brd][1]*16;
	for ($y=0; $y<16; $y++) {
	    for ($x=0; $x<32; $x++) {
		$c=imagecolorat($im,$x+$xx,$y+$yy);
		$data.=chr(($c>>17)&0x3f);
	    }
	}
    }
    echo("Writing... ".strlen($data)." bytes.\n");
    fwrite($com,$data);
}

?>