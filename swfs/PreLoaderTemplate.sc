.flash name=PreLoaderTemplate.swf background=white version=6
    .sprite loader # to be replaced by swfcombiner
    .end
    .put loader
    .frame 1
        .action:
            if(this.framesloaded < 2) {
                GotoFrame(0);
		Play();
            }
        .end
    .frame 2
    .sprite movie # to be replaced by swfcombine
    .end
    .put movie
    .frame 3
.end


