./bedtree -rq -d ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100
./bedtree -rq -g ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-author.txt 2948929 4 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-author.txt 2948929  ../query/query-dblp-author-1.txt 100 3
./createMismatchFilter ../data/data-dblp-author.txt index 3 1 2> temp.txt
#./queryMismatchFilter ../query/query-dblp-author-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100
./bedtree -rq -g ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-dblp-title.txt 1158648 4 300000 60000 ../query/query-dblp-title-1.txt 100 2 4 10
#./flamingo -r ../data/data-dblp-title.txt 1158648  ../query/query-dblp-title-1.txt 100 3
./createMismatchFilter ../data/data-dblp-title.txt index 3 1 2> temp.txt
#./queryMismatchFilter ../query/query-dblp-title-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-imdb-actor.txt 1213346 4 300000 60000 ../query/query-imdb-actor-1.txt 100
./bedtree -rq -g ../data/data-imdb-actor.txt 1213346 4 300000 60000 ../query/query-imdb-actor-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-imdb-actor.txt 1213346 4 300000 60000 ../query/query-imdb-actor-1.txt 100 2 4 10
#./flamingo -r ../data/data-imdb-actor.txt 1213346  ../query/query-imdb-actor-1.txt 100 3
./createMismatchFilter ../data/data-imdb-actor.txt index 3 1 2> temp.txt
#./queryMismatchFilter ../query/query-imdb-actor-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100
./bedtree -rq -g ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100 2 4 10
#./flamingo -r ../data/data-imdb-movie.txt 1568893  ../query/query-imdb-movie-1.txt 100 3
./createMismatchFilter ../data/data-imdb-movie.txt index 3 1 2> temp.txt
#./queryMismatchFilter ../query/query-imdb-movie-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100
./bedtree -rq -g ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100 2 4 10
./bedtree -rq -a ../data/data-uniprot.txt 508038 8 300000 60000 ../query/query-uniprot-1.txt 100 2 4 10
#./flamingo -r ../data/data-uniprot.txt 425940  ../query/query-uniprot-1.txt 100 3
./createMismatchFilter ../data/data-uniprot.txt index 3 1 2> temp.txt
#./queryMismatchFilter ../query/query-uniprot-m.txt index 1> temp.txt
