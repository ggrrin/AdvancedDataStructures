
function dox { 
echo './main_sim.out n 2048 1 | ./sim.out $1 $2 > "output-$1_$2.log"'
./main_sim.out n 2048 1 | ./sim.out $1 $2 > "output-$1_$2.log"
}

dox 64 64
dox 64 1024 
dox 64 4096 
dox 512 512 
dox 4096 64
