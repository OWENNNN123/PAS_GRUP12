Program ini merupakan implementasi dari sistem Manajemen Multi-Level Marketing (MLM) menggunakan struktur data Binary Search Tree (BST). Program ini dirancanguntuk mengelola data anggota, transaksi pembelian, dan komisi dalam struktur MLM.

Latar Belakang:
Dalam sistem MLM, setiap anggota bisa merekrut anggota baru dan mendapat komisi dari penjualan yang dilakukan oleh anggota di bawahnya. Struktur ini sering digambarkan sebagai pohon di mana setiap anggota bisa memiliki cabang kiri dan kanan, yang mencerminkan dua anggota yang mereka rekrut.

Fungsi-fungsi Utama:
Penambahan Anggota (TambahAnggota):

Menambahkan anggota baru ke dalam sistem. Jika ID yang dimasukkan lebih kecil dari ID anggota saat ini, ia akan ditempatkan di subpohon kiri, jika lebih besar, maka di subpohon kanan. Setiap anggota baru mungkin memiliki sponsor yang merupakan anggota yang mereferensikannya.
Pembelian Paket Dagang (BeliPaket):

Mencatat pembelian paket oleh anggota tertentu. Pembelian ini bisa mempengaruhi komisi yang diterima oleh anggota yang menjadi sponsor.
Penampilan dan Pencarian Anggota:

Menampilkan seluruh anggota yang ada dalam sistem dan mencari anggota berdasarkan ID atau nama. Pencarian dilakukan dengan mengunjungi setiap simpul dari BST sesuai dengan kriteria yang diinginkan.
Update Data Anggota (UpdateAnggota):

Memperbarui informasi dari anggota yang sudah ada, seperti nama dan jumlah pembelian.
Hapus Anggota (HapusAnggota):

Menghapus anggota dari sistem. Jika anggota yang dihapus memiliki subpohon, maka subpohon tersebut akan diatur kembali untuk memastikan BST tetap berlaku.
Penyimpanan dan Pemuatan Data (MuatData dan SimpanData):

Memuat data anggota dari file ketika program dijalankan dan menyimpan semua perubahan ke dalam file ketika program selesai.
Laporan Keuangan (LaporanKeuangan):

Menampilkan laporan mengenai total komisi, total anggota, dan rata-rata jumlah beli dari seluruh anggota.
Tampilan Struktur BST (PrintTree):

Secara visual menampilkan struktur pohon BST untuk memberikan gambaran yang lebih baik tentang bagaimana anggota saling terkait.
Statistik Keseluruhan (DisplayStats):

Menampilkan statistik keseluruhan dari sistem, termasuk jumlah total anggota, total komisi yang dibayarkan, dan jumlah rata-rata pembelian.
