# monkey.sc
#

.flash bbox=autocrop version=4 fps=50 name="HoppingMonkey.swf"

    .swf island "island.swf"
    .swf monkey "monkey4.swf"
    .put island
    .point leftfoot 45 177
    .point rightfoot 138 177
    .frame 0 .put monkey 160 160
    .frame 20 .change monkey 160 160
    .frame 30 .change monkey 160 122
    .frame 35 .change monkey 160 112
    .frame 40 .change monkey 160 122
    .frame 50 .change monkey 160 160
    
    .frame 70 .change monkey 205 337 pin=leftfoot rotate=0 
    .frame 90 .change monkey rotate=30
    .frame 100 .change monkey rotate=30
    .frame 120 .change monkey rotate=0
    .frame 122 .change monkey 298 337 pin=rightfoot rotate=0
    .frame 140 .change monkey rotate=-30
    .frame 150 .change monkey rotate=-30
    .frame 170 .change monkey rotate=0
.end
