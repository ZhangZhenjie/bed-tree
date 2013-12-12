./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 508038  ../query/query-uniprot-1.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 1 2> temp.txt
./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-2.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-2.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-2.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 508038  ../query/query-uniprot-2.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 2 2> temp.txt
./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-4.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-4.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-4.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 508038  ../query/query-uniprot-4.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 4 2> temp.txt
./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-8.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-8.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-8.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 508038  ../query/query-uniprot-8.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 8 2> temp.txt
./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-16.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-16.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-16.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 508038  ../query/query-uniprot-16.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 16 2> temp.txt
./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
