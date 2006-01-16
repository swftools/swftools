use File::Find;
use strict;

my $file = $ARGV[0];
if(!$file) {
    print "Usage: parsedoc.pl <docfile.doc>\n";
    exit(0);
}

my $paramhelp = "";

sub add_option_to_manpage
{
    my $shortopt = shift;
    my $longopt = shift;
    my $params = shift;
    my $doc = shift;
    my $longdoc = shift;
    chomp $longdoc;
    chomp $longdoc;
    $shortopt =~ s|-|\\-|; 
    $params =~ s|<(.*)>|\\fI\1\\fR|;
    $longdoc =~ s/<([a-zA-Z0-9_-]+)>/\\fI\1\\fR/;
    #$longdoc =~ s/%/%%$1/g;
    print manpage << "EOF"
.TP
\\fB\\-$shortopt\\fR, \\fB\\-\\-$longopt\\fR $params
$longdoc
EOF
;
}

sub add_option_to_help
{
    my $shortopt = shift;
    my $longopt = shift;
    my $params = shift;
    my $doc = shift;
    my $longdoc = shift;
    chomp $doc;
    chomp $doc;
    my $s1 = " "x(2-length($shortopt));
    my $s2 = " "x(20-length($longopt." ".$params));
    $doc =~ s/"/\\"/g;
    $paramhelp .= "    printf(\"-$shortopt$s1, --$longopt $params$s2$doc\\n\");\n";
}

sub add_header_to_manpage
{
    my $package = shift;
    my $progname = shift;
    my $short_description = shift;
    my $long_description = shift;
    my $usage = shift;
    my $date = `date +"%B %Y"`;
    my $newusage = "";
    chomp $date;

    split(/\n/,$usage);
    my $p  = "";
    foreach my $u (@_) {
	$u =~ s/%s/$progname/g;
	$newusage .= "$p.B $u\n";
	$p = ".PP\n";
    }

    print manpage << "EOF"
.TH $progname "1" "$date" "$progname" "$package"
.SH NAME
$progname - $short_description
.SH Synopsis
$newusage
.SH DESCRIPTION
$long_description
.SH OPTIONS
EOF
;
}
open(fi, "$file") or die "File not found";

my $package = "swftools";
my $progname = $file;
$progname =~ s|(^/?([^/]+/))*([^/]+).doc|$3|;
my $path = $1;

open(manpage, ">$path$progname.1") or die;

my $usage = "";
my $short_description = "";
my $long_description  = "";
while(<fi>) {
    last if(/^\s*$/);
    s/^\s*Usage:\s*//g;
    s/^\s*OR:\s*//g;
    $usage .= $_;
}
while(<fi>) {
    last if(/^\s*$/);
    $short_description .= $_;
}
while(<fi>) {
    last if(/^\s*$/);
    $long_description .= $_;
}

add_header_to_manpage($package, $progname, $short_description, $long_description, $usage);

$_=<fi>;

my $numopts = 0;
my @shortopts;
my @longopts;

while(1) {
    if(/^-([^\s,]+)[\s,]+--([^\s]+)(\s+(.*))?$/) {
	my $shortopt = $1;
	my $longopt = $2;
	my $params = $4;
	my $doc = "";
	my $longdoc = "";
	while(<fi>) {
	    last if(/^-/);
	    last if(/^\s*$/);
	    if(!$doc) {
		$doc .= $_;
	    } else {
		$longdoc .= $_;
	    }
	}
	$longdoc = $doc if(!$longdoc || ($longdoc =~ /^\s*$/));

	$shortopts[$numopts] = $shortopt;
	$longopts[$numopts] = $longopt;
	$numopts++;

	add_option_to_manpage($shortopt,$longopt,$params,$doc,$longdoc);
	add_option_to_help($shortopt,$longopt,$params,$doc,$longdoc);
	last if(eof(fi));
	next;
    }
    last if(/^\s*/);
    last if(!($_=<fi>));
    last if(/^\s*/);
}

my $tail = "";
while(<fi>) {
    $tail .= $_;
}

print manpage $tail;
close(manpage);


my $filename = "$path$progname.c";
my $cfile;
if(!open($cfile, "<$filename")) {
    $filename = "$path$progname.cpp";
    if(!open($cfile, "<$filename")) {
	$filename = "$path$progname.cc";
	if(!open($cfile, "<$filename")) {
	    $filename = "$path$progname.c++";
	    open($cfile, "<$filename") or die;
	}
    }
}

my $src="";
while(<$cfile>)
{
    if(/void\s*args_callback_usage.*\(.*\*/) {
	print "substituting usage in $filename\n";
	if(!/{/) {<$cfile>;}
	$src .= "void args_callback_usage(char *name)\n";
	$src .= "{\n";
	$src .= "    printf(\"\\n\");\n";
	while(<$cfile>) {
	    last if(/}\s*$/);
	}
	split(/\n/,$usage);
	my $prefix = "Usage:";
	my $u;
	foreach $u (@_) {
	    if($u =~ /%s/) {
		$src .= "    printf(\"$prefix $u\\n\", name);\n";
	    } else {
		$src .= "    printf(\"$prefix $u\\n\");\n";
	    }
	    $prefix = "OR:   ";
	}
	$src .= "    printf(\"\\n\");\n";

	$src .= "$paramhelp";
	
	$src .= "    printf(\"\\n\");\n";

	$src .= "}\n";
	next;
    }
    if(/^\s*(static\s+)?(struct\s+)?options_t.*=/) {
	print "substituting options_t in $filename\n";
	while(<$cfile>) {
	    last if(/;$/);
	}
	split(/\n/,$usage);
	$src .= "static struct options_t options[] = {\n";
	for(my $t=0;$t<$numopts;$t++) {
	    my $s = $shortopts[$t];
	    my $l = $longopts[$t];
	    $src .= "{\"$s\", \"$l\"},\n";
	}
	$src .= "{0,0}\n};\n";
	next;
    }
    $src .= $_;
}

close($cfile);
open($cfile, ">$filename") or die;
print $cfile $src;
close($cfile);

