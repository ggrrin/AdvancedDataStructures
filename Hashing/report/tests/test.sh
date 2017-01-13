r=../../../x64/Release/Hashing.exe

echo '( cd time_test ; ./test.sh $r )'
( cd time_test ; ./test.sh $r )

echo '( cd ./steps_test/ ; ./test.sh $r )'
( cd ./steps_test/ ; ./test.sh $r )

echo '( cd ./sequence_test/ ; ./test.sh $r )'
( cd ./sequence_test/ ; ./test.sh $r )

echo making graphs
( cd ./time_test/ ; ./graph.sh )
( cd ./steps_test/ ; ./graph.sh )
( cd ./sequence_test/ ; ./graph.sh )
