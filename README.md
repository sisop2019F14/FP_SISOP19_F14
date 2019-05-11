# FP_SISOP19_F14

SOAL


Buatlah sebuah music player dengan bahasa C yang memiliki fitur play nama_lagu, pause, next, prev, list lagu. Selain music player juga terdapat FUSE untuk mengumpulkan semua jenis file yang berekstensi .mp3 kedalam FUSE yang tersebar pada direktori /home/user. Ketika FUSE dijalankan, direktori hasil FUSE hanya berisi file .mp3 tanpa ada direktori lain di dalamnya. Asal file tersebut bisa tersebar dari berbagai folder dan subfolder. program mp3 mengarah ke FUSE untuk memutar musik.

Jawaban:

Langkah: Membuat dua source code, yang pertama adalah source code untuk melakukan mount dan satu lagi digunakan untuk menjalankan music player

1. mount.c

Terdapat fungsi xmp_getattr untuk mengambil atribut, xmp_readdir untuk membaca directory, xmp_read untuk membaca file, findsong untuk melakukan pencarian file yang berekstensi .mp3, copyfile untuk menyalin file.

2. play_mp3.c

Terdapat fungsi playsong yang digunakan untuk menjalankan lagu, listsong digunakan untuk menampikan list lagu, initSong (menyimpan nama lagu), findSongIndex(mencari index lagu), getCountSong(menghitung lagu) yang berguna untuk fiture next dan prev lagu. Main untuk menjalankan thread, input untuk menjalankan fiture berupa play, pause, next, prev, dan list lagu.
