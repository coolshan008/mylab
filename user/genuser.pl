#!/usr/bin/perl

open(SIG, $ARGV[0]) || die "open $ARGV[0]: $!";

$n = sysread(SIG, $buf, 1024);

if($n > 1024){
  print STDERR "ERROR:This user binary file too large: \
	  $n bytes (max 1024)\n";
  exit 1;
}

print STDERR "This user binary file is $n bytes \n";

$m = $n % (512);

#$buf .= "\0" x ( 512 - $m);


open(SIG, ">$ARGV[0]") || die "open >$ARGV[0]: $!";
print SIG $buf;
close SIG;


open(SIG, $ARGV[0]) || die "open $ARGV[0]: $!";

$n = sysread(SIG, $buf,1024);

if($n > 1024){
  print STDERR "ERROR: This user binary file too large:\
	     $n bytes (max 1024)\n";
  exit 1;
}

print STDERR "This user binary file is $n bytes (max 510)\n";
print STDERR "*********************************************\n";
print STDERR "*********************************************\n";
print STDERR "*********************************************\n";

close SIG;
