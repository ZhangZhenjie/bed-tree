./bedtree -rj -g ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 1    
./mismatchFilterJoin ../data/data-imdb-movie.txt 3 1
./bedtree -rj -g ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 2
./mismatchFilterJoin ../data/data-imdb-movie.txt 3 2
./bedtree -rj -g ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 3
./mismatchFilterJoin ../data/data-imdb-movie.txt 3 3
./bedtree -rj -g ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10 4
./mismatchFilterJoin ../data/data-imdb-movie.txt 3 4
