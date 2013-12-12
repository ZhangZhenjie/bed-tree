./bedtree -rj -g ../data/data-uniprot.txt 508038 16 300000 60000 ../query/query-uniprot-1.txt 100 2 4 10 1
./mismatchFilterJoin ../data/data-uniprot.txt 3 1
./bedtree -rj -g ../data/data-uniprot.txt 508038 16 300000 60000 ../query/query-uniprot-2.txt 100 2 4 10 2
./mismatchFilterJoin ../data/data-uniprot.txt 3 2
./bedtree -rj -g ../data/data-uniprot.txt 508038 16 300000 60000 ../query/query-uniprot-4.txt 100 2 4 10 3
./mismatchFilterJoin ../data/data-uniprot.txt 3 3
./bedtree -rj -g ../data/data-uniprot.txt 508038 16 300000 60000 ../query/query-uniprot-8.txt 100 2 4 10 4
./mismatchFilterJoin ../data/data-uniprot.txt 3 4
