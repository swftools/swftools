while(<stdin>)
{
    if(/\[CALLPERL .*left\]/ ... /\[CALLPERL end\]/) {
	$code .= $_ if(!/CALLPERL/);
	$name = $1 if(/.flash.*name=([^&][^ ]*)/);
	$name = $1 if(/.flash.*name=&quot;([^&]*)&quot;/);
	chomp $name;
    } 
    elsif(/\[CALLPERL .*right\]/ ... /\[CALLPERL end\]/) {
	$highlight .= $_ if(!/CALLPERL/);
    }
    elsif ($code ne "") {
	$code =~ s/&quot;/"/g;

#	    print stderr "Warning: ttf->swf\n" if($code =~ s/Arial.ttf/Arial.swf/g);

	open(fi, ">tmp.sc");print fi $code;close(fi);
	print stderr "swfc tmp.sc ($name)\n";
	system("swfc tmp.sc >&2");
	($embed = `swfdump -e $name`) =~ /WIDTH="([^"]*)"/;
	$width = $1;
	print "<td bgcolor=\"#ffffff\" width=\"$width\">";
	print $embed;
	print "</td>";
	$code="";
	print;
	unlink "tmp.sc";
    }
    elsif ($highlight ne "") {
	$highlight =~ s/^\n\s*//g;
	$highlight =~ s/\s*\n$//g;
	print "<pre>\n";
	# todo: apply syntax highlighting.
	print $highlight."\n";
	print "</pre>\n";
	$highlight="";
	print;
    }
    else {
	print;
    }
}
