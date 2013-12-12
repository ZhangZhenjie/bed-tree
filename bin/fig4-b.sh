./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-1.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 1 2> temp.txt
./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-2.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-2.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-2.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-2.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 2 2> temp.txt
./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-4.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-4.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-4.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-4.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 4 2> temp.txt
./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-8.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-8.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-8.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-8.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 8 2> temp.txt
./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-16.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-16.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-16.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-16.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 16 2> temp.txt
./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
