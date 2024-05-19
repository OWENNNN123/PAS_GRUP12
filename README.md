**Fungsi/Modular**
Program udh modular dengan pemisahan fungsi untuk penciptaan anggota, penambahan ke BST, pencarian, pembelian paket, penyimpanan dan pembacaan data, ama  menu interaktif.

**Error Handling**

Program mengecek apakah anggota ada sebelum memproses pembelian.
Program mengecek file ketika memuat data.
tpi blum ada penanganan untuk:

1. Input non-numerik saat diminta ID atau pilihan menu.
2. Menangani kegagalan pembukaan file untuk ditulis.
3. Untuk meningkatkan error handling, bisa ditambahin pemeriksaan lebih lanjut untuk input dan memastikan  semua operasi file dilakukan dengan benar, dengan memberikan pesan kesalahan yang tepat jika terjadi kegagalan.
4. kalau gak mager menunya cakepin lagi hehe
   
**Berbasis Dynamic Data Structure**
udah pake struktur data dinamis yaitu Binary Search Tree (BST) untuk menyimpan anggota.

**Memiliki Fungsi Searching/Sorting**
Searching: udah ada fungsi untuk mencari anggota berdasarkan ID menggunakan pencarian dalam BST.
Sorting: BST secara inheren menjaga elemen-elemen dalam urutan terurut berdasarkan ID. tpi blum ada fungsi eksplisit untuk mengurutkan data berdasarkan kriteria lain seperti nama atau jumlah beli.
File Handling
Program sudah memiliki kemampuan untuk membaca dan menulis data ke file, yang mengizinkan penyimpanan persisten anggota antarsesi.

**Parallel Processing**
belum ada, bisa tambahin kek

1.Meningkatkan pencarian atau operasi lain pada BST, terutama jika data menjadi sangat besar.
2. Menangani operasi file secara asinkron atau memproses input/output dalam thread terpisah dari logika utama program.
