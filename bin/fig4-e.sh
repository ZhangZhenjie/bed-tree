./bedtree -rq -d ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100 
./bedtree -rq -g ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100 2 4 10 
./bedtree -rq -a ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-1.txt 100 2 4 10 
#./flamingo -r ../data/data-imdb-movie.txt 1568893  ../query/query-imdb-movie-1.txt 100 3
./createMismatchFilter ../data/data-imdb-movie.txt index 3 1 2> temp.txt           
./queryMismatchFilter ../query/query-imdb-movie-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-2.txt 100 
./bedtree -rq -g ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-2.txt 100 2 4 10 
./bedtree -rq -a ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-2.txt 100 2 4 10 
#./flamingo -r ../data/data-imdb-movie.txt 1568893  ../query/query-imdb-movie-2.txt 100 3
./createMismatchFilter ../data/data-imdb-movie.txt index 3 2 2> temp.txt 
./queryMismatchFilter ../query/query-imdb-movie-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-3.txt 100 
./bedtree -rq -g ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-3.txt 100 2 4 10 
./bedtree -rq -a ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-3.txt 100 2 4 10 
#./flamingo -r ../data/data-imdb-movie.txt 1568893  ../query/query-imdb-movie-3.txt 100 3
./createMismatchFilter ../data/data-imdb-movie.txt index 3 3 2> temp.txt
./queryMismatchFilter ../query/query-imdb-movie-m.txt index 1> temp.txt
./bedtree -rq -d ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-4.txt 100 
./bedtree -rq -g ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-4.txt 100 2 4 10 
./bedtree -rq -a ../data/data-imdb-movie.txt 1568893 4 300000 60000 ../query/query-imdb-movie-4.txt 100 2 4 10 
#./flamingo -r ../data/data-imdb-movie.txt 1568893  ../query/query-imdb-movie-4.txt 100 3
./createMismatchFilter ../data/data-imdb-movie.txt index 3 4 2> temp.txt
./queryMismatchFilter ../query/query-imdb-movie-m.txt index 1> temp.txt
