.macro fader TXT
        .frame 0
        .put TXT x=0 y=10 alpha=0     # mesg 1 placed 
        .frame 20
        .change TXT alpha=100%        # mesg 1 bright
        .frame 40
        .change TXT                   # mesg 1 hold
        .frame 60
        .change TXT alpha=0           # mesg 1 fade
        .frame 65
        .del TXT                      # mesg 1 gone
.end

