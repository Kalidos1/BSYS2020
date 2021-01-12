
# Overview

This homework lets you play around with a number of ways to implement
a small, deadlock-free vector object in C. The vector object is quite
limited (e.g., it only has `add()` and `init()` functions) but is just
used to illustrate different approaches to avoiding deadlock.

Some files that you should pay attention to are as follows. They, in
particular, are used by all the variants in this homework.
- `common_threads.h`: The usual wrappers around many different pthread (and other) library calls, so as to ensure they are not failing silently
- `vector-header.h`: A simple header for the vector routines, mostly defining a fixed vector size and then a struct that is used per vector (vector_t)
- `main-header.h`: A number of global variables common to each different program
- `main-common.c`: Contains the main() routine (with arg parsing) that initializes two vectors, starts some threads to access them (via a worker() routine), and then waits for the many vector_add()'s to complete

The variants of this homework are found in the following files. Each takes a
different approach to dealing with concurrency inside a "vector addition"
routine called `vector_add()`; examine the code in these files to get a sense of
what is going on. They all use the files above to make a complete runnable
program. 

The relevant files:
- `vector-deadlock.c`: This version blithely grabs the locks in a particular order (dst then src). By doing so, it creates an "invitation to deadlock", as one thread might call `vector_add(v1, v2)` while another concurrently calls `vector_add(v2, v1)`.
- `vector-global-order.c`: This version of `vector_add()` grabs the locks in a total order, based on address of the vector. 
- `vector-try-wait.c`: This version of `vector_add()` uses `pthread_mutex_trylock()` to attempt to grab locks; when the try fails, the code releases any locks it may hold and goes back to the top and tries it all over again.
- `vector-avoid-hold-and-wait.c`: This version ensures it can't get stuck in a hold and wait pattern by using a single lock around lock acquisition.
- `vector-nolock.c`: This version doesn't even use locks; rather, it uses an atomic fetch-and-add to implement the `vector_add()` routine. Its semantics (as a result) are slightly different.

Type `make` (and read the `Makefile`) to build each of five executables. 

```sh
prompt> make
```

Then you can run a program by simply typing its name:

```sh
prompt> ./vector-deadlock
```

Each program takes the same set of arguments (see main-common.c for details):
- `-d`: This flag turns on the ability for threads to deadlock. When you pass `-d` to the program, every other thread calls `vector_add()` with the vectors in a different order, e.g., with two threads, and `-d` enabled, Thread 0 calls `vector_add(v1, v2)` and Thread 1 calls `vector_add(v2, v1)`
- `-p`: This flag gives each thread a different set of vectors to call add upon, instead of just two vectors. Use this to see how things perform when there isn't contention for the same set of vectors.
- `-n num_threads`: Creates some number of threads; you need more than one to deadlock.
- `-l loops`: How many times should each thread call add?
- `-v`: Verbose flag: prints out a little more about what is going on.
- `-t`: Turns on timing and shows how long everything took.


_____________QUESTIONS_______________


Question 1: vector-deadlock.c

(venv) ➜  threads-bugs git:(master) ✗ ./vector-deadlock -n 2 -l 1 -v 
->add(0, 1)
<-add(0, 1)
              ->add(0, 1)
              <-add(0, 1)
(venv) ➜  threads-bugs git:(master) ✗ ./vector-deadlock -n 2 -l 1 -v -t 
              ->add(0, 1)
              <-add(0, 1)
->add(0, 1)
<-add(0, 1)


- Jenachdem welcher Thread als erstes abläuft
- 


Question 2: vector-deadlock.c

- ./vector-deadlock -n 2 -l 100000 -v -d

- Mit 100000 Wiederholungen tritt fast immer ein Deadlock ein, jedoch kann es auch passieren, dass mal kein Deadlock eintritt


Question 3: vector-deadlock.c

- Mit -n 1 kann kein Deadlock auftreten


- Bei 10 Threads, kommt es auch bei 10000 Loops zu einem Deadlock, was bei 2 Threads nicht passiert ist



Question 4: vector-global-order.c

- Der Code versucht hier mit der IF-Condition zu gewährleisten, dass wenn ein Vector_Add aufgerufen wird gleichermaßen die Locks aufgerufen werden
-> Nicht vector_add(Lock1, Lock2) und vector_add(Lock2, Lock1) was zu einem Deadlock führen kann , sondern dass die Reihenfolge der Locks stimmt -> L1, L2 und L1, L2

-> Garantiert immer eine einheitliche Reihenfolge



- Wenn src + dst gleich sind, brauch man kein Extra Lock -> Dadurch, verringert man die Gefahr, dass hierbei andere Threads beeinträchtigt werden
- 2 Mal Locking ??? VErringerte Performance ?
????


Question 5: vector-global-order.c

-  ./vector-global-order -t -n 2 -l 100000 -d
Time: 0.11 seconds

- ./vector-global-order -t -n 2 -l 1000000 -d
  Time: 1.04 seconds

- ./vector-global-order -t -n 10 -l 100000 -d
  Time: 0.47 seconds


-> In beiden Fällen dauert das Programm länger bis alles abgeschlossen ist




Question 6: vector-global-order.c


- Viel bessere Performance, wenn die Threads unabhängig von den Vektoren arbeiten
- Dadurch wird auch das benutzen der Threads viel besser und mehrere Threads werden nicht viel langsamer

./vector-global-order -t -n 2 -l 100000 -d -p
Time: 0.03 seconds

./vector-global-order -t -n 2 -l 1000000 -d -p
Time: 0.37 seconds

./vector-global-order -t -n 10 -l 100000 -d -p
Time: 0.07 seconds



Question 7: vector-try-wait.c


- Man benötigt das erste Lock, da später, die Werte der dst geändert werden, und wenn dieses Lock nicht freigegeben ist, könnte eine Race Condition auftreteten ?


- ./vector-try-wait -t -n 2 -l 100000 -d
  Retries: 456753
  Time: 0.20 seconds
  an851goe@ct-bsys-ws20-18:~/z-drive/BSYS2020/Chapter32/threads-bugs$ ./vector-try-wait -t -n 2 -l 100000 -d
  Retries: 421742
  Time: 0.18 seconds

-> Es ist manchmal deutlich langsamer als Global-Order, jedoch gibt es auch fälle ohne Retries, was sehr nah an die andere Zeit herankommt
-> Retries gehen sehr viel höher, wenn man die Threads erhöht, was dementsprechen auch sehr lange dauert:
./vector-try-wait -t -n 5 -l 100000 -d
    Retries: 2640613
    Time: 3.01 seconds
an851goe@ct-bsys-ws20-18:~/z-drive/BSYS2020/Chapter32/threads-bugs$ ./vector-try-wait -t -n 10 -l 100000 -d
    Retries: 14483069
    Time: 18.63 seconds
    
    
    
Question 8: vector-avoid-hold-and-wait.c


- Das Problem hierbei ist, das jedesmal mit einem Globalen Lock, alle benötigten Locks erhalten werden, was die Performance beeinträchtigt
- Jedoch hat es eine gewisse Sicherheit, da immer die gleiche Reihenfolge verwendet wird


- Ohne -p:
    -> Langsamer als Global-Order, jedoch gleich auf mit vector-try-wait

- Mit -p:
    -> Mit dem Parallelen, kommt es nah und manchmal sogar gleich auf mit dem Global-Order


Question 9: vector-nolock.c

- Ja, es ist mehr oder weniger dasselbe, da Fetch-and-add eine Funktion ist, die Atomisch abläuft, genau wie Compare and Swap als Beispiel in dem Buch
-> Dadurch hat dies genau die selbe Funktion, wie ein Lock

Question 10: vector-nolock.c

/vector-nolock -t -n 2 -l 100000 -d
Time: 0.67 seconds
an851goe@ct-bsys-ws20-18:~/z-drive/BSYS2020/Chapter32/threads-bugs$ ./vector-nolock -t -n 2 -l 100000 -d
Time: 0.62 seconds

- Ohne -p:
    -> Dauert erheblich länger als Global-Order oder die anderen Lock Beispiele
    
- Mit -p:
    -> Mit dem Parallelism gleicht es sich den anderen Programmen an, die mit Locks arbeiten
    -> Mit  erhöhenden Threads wird es im Vergleich zu den anderen nicht viel langsamer -> Lineares SKalieren?
