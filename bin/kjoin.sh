./bedtree -kj -d ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 4    
./bedtree -kj -d ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 4    
./bedtree -kj -d ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 4    
./bedtree -kj -d ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 4    
./bedtree -kj -d ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 4    
./bedtree -kj -d ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 16    
./bedtree -kj -d ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 16    
./bedtree -kj -d ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 16    
./bedtree -kj -d ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 16    
./bedtree -kj -d ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 16    
./bedtree -kj -d ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 64    
./bedtree -kj -d ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 64    
./bedtree -kj -d ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 64    
./bedtree -kj -d ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 64    
./bedtree -kj -d ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 64    
./bedtree -kj -d ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 256    
./bedtree -kj -d ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 256    
./bedtree -kj -d ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 256    
./bedtree -kj -d ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 256    
./bedtree -kj -d ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 256    
# top-k join with gram order
./bedtree -kj -ng ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 4
./bedtree -kj -ng ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 4
./bedtree -kj -ng ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 4
./bedtree -kj -ng ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 4
./bedtree -kj -ng ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 4
./bedtree -kj -ng ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 16
./bedtree -kj -ng ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 16
./bedtree -kj -ng ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 16
./bedtree -kj -ng ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 16 
./bedtree -kj -ng ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 16
./bedtree -kj -ng ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 64
./bedtree -kj -ng ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 64
./bedtree -kj -ng ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 64
./bedtree -kj -ng ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 64
./bedtree -kj -ng ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 64
./bedtree -kj -ng ../data/data-dblp-author.txt 696361 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 256
./bedtree -kj -ng ../data/data-dblp-title.txt 1125454 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 256
./bedtree -kj -ng ../data/data-imdb-actor.txt 1213346 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 256
./bedtree -kj -ng ../data/data-imdb-movie.txt 373814 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 256
./bedtree -kj -ng ../data/data-uniprot.txt 425940 8 300000 60000 ../query/query-dblp-author-1.txt 100 2 4 11 256
