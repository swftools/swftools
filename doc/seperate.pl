#!/usr/bin/perl

$filename = $ARGV[0];

$filename =~ /^(.*)\.([^.]*)$/;
$basename = $1;
$extension = $2;

print "Processing $filename...\n";

$head = "";

open(fi, "<$filename");

while(<fi>) {
    last if(/snip:chapter/);
    $head .= $_;
}

# chapters
my $nr=1;
my @chapters;
my @names;
/!!!"(.*)"!!!/;$name = $1;
while(<fi>) {
    my $chapter = "";
    # one chapter
    while(1) {
	if(/snip:\/chapter/) {
	    while(<fi>) {
		last if(!/^\s*$/);
	    }
	    last;
	}
	$chapter .= $_;
	$_ = <fi>;
    }
    $names[$nr] = $name;
    $chapters[$nr++] = $chapter;

    /!!!"(.*)"!!!/;$name = $1;
    if(!/snip:chapter/) {
	last;
    }
}

while($_) {
    $tail .= $_;
    $_ = <fi>;
}

close(fi);

my $num = $nr;
$nr=1;
for($nr=1;$nr<$num;$nr++) {
    $h2 = $head;
    $t2 = $tail;
    my $filename = "$basename.$nr.$extension";
    print "Creating $filename...\n";
    for($n=1;$n<$num;$n++) {
	if($nr == $n) {
	    $h2 =~ s/chapterlink$n/<a href="$basename.$n.$extension">&gt;$names[$n]&lt;<\/a>/;
	} else {
	    $h2 =~ s/chapterlink$n/<a href="$basename.$n.$extension">$names[$n]<\/a>/;
	}
    }
  
    if($nr > 1) {
	$prev = $names[$nr-1];
	$aprev = $basename.".".($nr-1).".".$extension;
	$t2 =~ s/__prevlink__/<a href="$aprev">Previous:&nbsp;$prev<\/a>/;
    } else {
	$t2 =~ s/__prevlink__/&nbsp;/;
    }
    if($nr < $num) {
	$next= $names[$nr+1];
	$anext = $basename.".".($nr+1).".".$extension;
	
	$t2 =~ s/__nextlink__/<a href="$anext">Next:&nbsp;$next<\/a>/;
    } else {
	$t2 =~ s/__nextlink__/&nbsp;/;
    }
    $t2 =~ s/__currentchapter__/$names[$nr]/;

    open(fi, ">$filename");
    print fi "$h2\n$chapters[$nr]\n$t2";
    close(fi);
}
