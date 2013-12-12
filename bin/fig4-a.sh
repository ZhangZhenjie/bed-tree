./bedtree -rq -d ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100 
./bedtree -rq -g ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 
./bedtree -rq -a ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 
#./flamingo -r ../data/data-dblp-author.txt 2948929  ../query/query-dblp-author-1.txt 100 3
./createMismatchFilter ../data/data-dblp-author.txt index 3 1 2> temp.txt           
./queryMismatchFilter ../query/query-dblp-author-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-2.txt 100 
./bedtree -rq -g ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-2.txt 100 2 4 10 
./bedtree -rq -a ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-2.txt 100 2 4 10 
#./flamingo -r ../data/data-dblp-author.txt 2948929  ../query/query-dblp-author-2.txt 100 3
./createMismatchFilter ../data/data-dblp-author.txt index 3 2 2> temp.txt 
./queryMismatchFilter ../query/query-dblp-author-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-3.txt 100 
./bedtree -rq -g ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-3.txt 100 2 4 10 
./bedtree -rq -a ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-3.txt 100 2 4 10 
#./flamingo -r ../data/data-dblp-author.txt 2948929  ../query/query-dblp-author-3.txt 100 3
./createMismatchFilter ../data/data-dblp-author.txt index 3 3 2> temp.txt
./queryMismatchFilter ../query/query-dblp-author-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-4.txt 100 
./bedtree -rq -g ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-4.txt 100 2 4 10 
./bedtree -rq -a ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-4.txt 100 2 4 10 
#./flamingo -r ../data/data-dblp-author.txt 2948929  ../query/query-dblp-author-4.txt 100 3
./createMismatchFilter ../data/data-dblp-author.txt index 3 4 2> temp.txt
./queryMismatchFilter ../query/query-dblp-author-m.txt index 1> temp.txt
