sistem pemasaran berjenjang (MLM), di mana pengguna dapat mengelola anggota yang direpresentasikan sebagai node dalam pohon pencarian biner (BST). Setiap node anggota berisi informasi seperti ID, nama, jumlah pembelian, total keuntungan, ID referensi, dan komisi. Fungsionalitas utama termasuk menambahkan dan memperbarui data anggota, menghitung komisi, dan menghasilkan laporan. Program ini juga menggunakan OpenMP untuk memparalelkan beberapa operasi, meningkatkan kinerja untuk tugas seperti pencarian anggota dan agregasi statistik.

Berikut adalah penjelasan tentang cara kerja kode, dengan fokus pada setiap fungsi dan operasi utama:

Struktur Data
Anggota: Ini adalah struktur untuk setiap anggota dalam sistem MLM. Ini mengandung bidang untuk ID, nama, jumlah pembelian, total keuntungan, dan pointer pohon biner (kiri dan kanan untuk anak kiri dan kanan), pengenal referensi, dan komisi.
Fungsi Inti
cariAnggota: Mencari pohon pencarian biner untuk anggota berdasarkan ID. Menggunakan pencarian rekursif, yang khas untuk operasi BST, untuk menavigasi melalui anak kiri atau kanan berdasarkan perbandingan ID.

cariAnggotaDenganNama: Mencari secara rekursif anggota berdasarkan nama di seluruh BST. Menggunakan tugas OpenMP untuk memparalelkan pencarian pada subtree kiri dan kanan, menunggu semua tugas selesai dengan #pragma omp taskwait.

komisiReferal: Menghitung komisi referensi untuk anggota secara rekursif melalui pohon. Setiap kali anggota yang dirujuk cocok dengan referer_code, komisi mereka meningkat sebesar jumlah tetap (2600 unit).

updateAnggota: Memungkinkan pembaruan nama anggota dan jumlah pembelian berdasarkan ID mereka. Pertama-tama menemukan anggota menggunakan cariAnggota, kemudian meminta data baru untuk menimpa bidang yang ada.

printTree: Menampilkan secara visual struktur pohon secara horizontal, yang dapat membantu memvisualisasikan struktur BST.

displayStats: Mengumpulkan dan mencetak total anggota, total pembelian, dan total keuntungan. Menggunakan OpenMP untuk memparalelkan agregasi data rekursif, memastikan keselamatan benang dengan #pragma omp critical saat memperbarui variabel bersama.

tampilkanSemuaAnggota: Menampilkan detail semua anggota dengan melakukan traversal rekursif pohon. Memparalelkan traversal anak kiri dan kanan menggunakan bagian OpenMP.

tambahAnggota: Menambahkan anggota baru ke BST. Fungsi ini menempatkan node baru berdasarkan properti BST, memastikan pohon tetap berurutan berdasarkan ID.

simpanDataKeFile: Menyimpan data anggota ke file dalam format terstruktur dengan melakukan traversal pohon secara rekursif dan menulis detail setiap anggota.

prosesPembelian: Memproses pembelian untuk anggota, menyesuaikan jumlah total pembelian mereka dan menghitung keuntungan dan komisi berdasarkan berapa banyak item yang telah mereka beli sebelumnya.

hapusAnggota: Menghapus anggota dari BST. Menangani tiga kasus: menghapus node daun, node dengan satu anak, dan node dengan dua anak, mempertahankan properti BST setelah penghapusan.

Loop Utama
Loop utama menyajikan menu yang memungkinkan pengguna berinteraksi dengan sistem. Pengguna dapat memilih untuk menambahkan anggota, memperbarui data anggota, menampilkan semua anggota, menghitung dan menampilkan laporan keuangan, dan menyimpan data ke file. Loop ini berlanjut sampai pengguna memutuskan untuk keluar dengan memilih "Simpan dan Keluar".
Penggunaan OpenMP
Kode menggunakan pragma OpenMP (#pragma omp) untuk meningkatkan kinerja dengan memparalelkan tugas yang cocok. Ini termasuk mencari anggota, mengumpulkan statistik, dan menampilkan data anggota. Peningkatan ini sangat efektif untuk operasi yang melibatkan traversal pohon rekursif
