//Program MLM
//2306267164 - Andhika Fadhlan Wijanarko
//2306250604 - Rowen Rodotua Harahap

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define clear() printf("\033[H\033[J")

#define ALPHABET_SIZE 26

#define HARGA_DASAR 100 

typedef struct Anggota {
    int id;
    char nama[100], password[100], referer_id[5], referer_code[5];
    int jumlah_pembelian, komisi;
    double keuntunganTotal;
    struct Anggota *kiri, *kanan;
} Anggota;

Anggota *akar = NULL;

// Fungsi untuk mencari anggota menggunakan ID
Anggota* cariAnggota(Anggota *akar, int id) {
    if (akar == NULL || akar->id == id)
        return akar;
    if (id < akar->id)
        return cariAnggota(akar->kiri, id);
    return cariAnggota(akar->kanan, id);
}

// Fungsi untuk mencari anggota menggunakan nama
void cariAnggotaDenganNama(Anggota *akar, char *nama) {
    if (akar != NULL) {
        #pragma omp task shared(akar, nama)
        {
            cariAnggotaDenganNama(akar->kiri, nama);
        }
        if (strcmp(akar->nama, nama) == 0) {
            printf("ID: %d, Nama: %s, Pembelian: %d, Komisi %d\n", akar->id, akar->nama, akar->jumlah_pembelian, akar->komisi);
        }
        #pragma omp task shared(akar, nama)
        {
            cariAnggotaDenganNama(akar->kanan, nama);
        }
        #pragma omp taskwait
    }
}

// Fungsi untuk menambahkan komisi berdasarkan referer code
void komisiReferal(Anggota *akar, char *referer_id, int komisi) {
    if (akar != NULL) {
        #pragma omp task shared(akar, referer_id, komisi)
        {
            komisiReferal(akar->kiri, referer_id, komisi);
        }
        if (strcmp(akar->referer_code, referer_id) == 0) {
            akar->komisi += komisi;
            komisiReferal(akar, akar->referer_code, komisi/20);
        }
        #pragma omp task shared(akar, referer_id, komisi)
        {
            komisiReferal(akar->kanan, referer_id, komisi);
        }
        #pragma omp taskwait
    }
}

// Fungsi untuk memperbarui data anggota yang sudah ada
void updateAnggota() {
    int id, jumlah_pembelian_baru;
    char nama_baru[100];
    printf("Masukkan ID Anggota yang akan diupdate: ");
    scanf("%d", &id);
    Anggota *anggota = cariAnggota(akar, id);
    if (anggota == NULL) {
        printf("Anggota tidak ditemukan.\n");
        return;
    }
    printf("Update nama (sekarang: %s): ", anggota->nama);
    scanf(" %[^\n]s", nama_baru);
    strcpy(anggota->nama, nama_baru);
    printf("Update jumlah pembelian (sekarang: %d): ", anggota->jumlah_pembelian);
    scanf("%d", &jumlah_pembelian_baru);
    anggota->jumlah_pembelian = jumlah_pembelian_baru;
}

// Fungsi untuk mencetak pohon biner anggota
void printTree(Anggota* node, int space) {
    const int COUNT = 10;
    if (node == NULL) return;
    space += COUNT;
    printTree(node->kanan, space);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d(%s)\n", node->id, node->nama);
    printTree(node->kiri, space);
}

// Fungsi untuk menampilkan statistik keseluruhan
void displayStats() {
    int totalAnggota = 0;
    int totalPembelian = 0;
    double totalKeuntungan = 0;

    void aggregateStats(Anggota* node) {
        if (node != NULL) {
            #pragma omp atomic
            totalAnggota++;
            #pragma omp atomic
            totalPembelian += node->jumlah_pembelian;
            #pragma omp atomic
            totalKeuntungan += node->keuntunganTotal;
            #pragma omp task shared(node)
            {
                aggregateStats(node->kiri);
            }
            #pragma omp task shared(node)
            {
                aggregateStats(node->kanan);
            }
            #pragma omp taskwait
        }
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            aggregateStats(akar);
        }
    }
    printf("Total Anggota: %d\n", totalAnggota);
    printf("Total Pembelian: %d\n", totalPembelian);
    printf("Total Keuntungan: %.2f\n", totalKeuntungan);
}

// Fungsi untuk menampilkan semua anggota
void tampilkanSemuaAnggota(Anggota *node) {
    if (node != NULL) {
        tampilkanSemuaAnggota(node->kiri);
        printf("ID: %d, Nama: %s, Jumlah Pembelian: %d, Keuntungan Total: %.2f, Komisi Total: %d, password: %s, Kode Referral: %s, Input Referral: %s\n", 
               node->id, node->nama, node->jumlah_pembelian, node->keuntunganTotal, node->komisi, node->password, node->referer_code, node->referer_id);
        tampilkanSemuaAnggota(node->kanan);
    }
}

// Fungsi untuk menambahkan anggota baru
Anggota* tambahAnggota(Anggota *simpul, int id, char *nama, char *password, char *referer_id, char *referalIn) {
    if (simpul == NULL) {
        Anggota *simpulBaru = (Anggota*)malloc(sizeof(Anggota));
        simpulBaru->id = id;
        strcpy(simpulBaru->nama, nama);
        strcpy(simpulBaru->password, password);
        simpulBaru->jumlah_pembelian = 0;
        simpulBaru->keuntunganTotal = 0;
        simpulBaru->komisi = 0;
        simpulBaru->kiri = simpulBaru->kanan = NULL;
        strcpy(simpulBaru->referer_code, referalIn); 
        strcpy(simpulBaru->referer_id, referer_id);
        return simpulBaru;
    }
    if (id < simpul->id)
        simpul->kiri = tambahAnggota(simpul->kiri, id, nama, password, referer_id, referalIn);
    else if (id > simpul->id)
        simpul->kanan = tambahAnggota(simpul->kanan, id, nama, password, referer_id, referalIn);
    return simpul;
}

// Fungsi untuk menyimpan data anggota ke file
void simpanDataAnggotaKeFile(Anggota *node, FILE *file) {
    if (node != NULL) {
        simpanDataAnggotaKeFile(node->kiri, file);
        fprintf(file, "ID: %d, Nama: %s, Jumlah Pembelian: %d, Keuntungan Total: %.2f, Komisi Total: %d, password: %s, Kode Referral: %s, Input Referral: %s\n", 
                node->id, node->nama, node->jumlah_pembelian, node->keuntunganTotal, node->komisi, node->password, node->referer_code, node->referer_id);
        simpanDataAnggotaKeFile(node->kanan, file);
    }
}
// Fungsi untuk menyimpan data perusahaan ke file
void simpanDataPerusahaanKeFile(Anggota *node, FILE *file) {
    int totalAnggota = 0;
    int totalPembelian = 0;
    double totalKeuntungan = 0;

    void aggregateStats(Anggota* node) {
        if (node != NULL) {
            #pragma omp atomic
            totalAnggota++;
            #pragma omp atomic
            totalPembelian += node->jumlah_pembelian;
            #pragma omp atomic
            totalKeuntungan += node->keuntunganTotal;
            #pragma omp task shared(node)
            {
                aggregateStats(node->kiri);
            }
            #pragma omp task shared(node)
            {
                aggregateStats(node->kanan);
            }
            #pragma omp taskwait
        }
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            aggregateStats(akar);
        }
    }
        fprintf(file, "Total Anggota: %d\nTotal Pembelian: %d\nTotal Keuntungan: %.2f\n", 
                totalAnggota, totalPembelian, totalKeuntungan);
}

// Fungsi untuk memproses pembelian
void prosesPembelian(Anggota *ang, char *password) {
    if(strcmp(ang->password, password)==0){
      double hargaPembelian, komisi, keuntunganPerusahaan;
      if (ang->jumlah_pembelian == 0) {
          hargaPembelian = 2.0 * HARGA_DASAR;
          keuntunganPerusahaan = 0.55 * hargaPembelian;
          komisi = 0.5 * hargaPembelian;
          komisiReferal(ang, ang->referer_id, komisi); // Update komisi
      } else if (ang->jumlah_pembelian == 1) {
          hargaPembelian = HARGA_DASAR;
          keuntunganPerusahaan = 0.25 * hargaPembelian;
          // Tidak ada komisi untuk pembelian kedua
      } else {
          hargaPembelian = 0.8 * HARGA_DASAR;
          keuntunganPerusahaan = 0.05 * hargaPembelian;
          // Tidak ada komisi untuk pembelian ketiga dan seterusnya
      }
      printf("Harga Pembelian: %.2f\n", hargaPembelian);
      printf("Keuntungan Perusahaan: %.2f\n", keuntunganPerusahaan);
      ang->jumlah_pembelian++;
      ang->keuntunganTotal += keuntunganPerusahaan;
    }else{
      printf("login tidak valid\n");
    }
  }
// Fungsi untuk menghapus anggota
Anggota* hapusAnggota(Anggota *akar, int id) {
    if (akar == NULL) return akar;
    if (id < akar->id)
        akar->kiri = hapusAnggota(akar->kiri, id);
    else if (id > akar->id)
        akar->kanan = hapusAnggota(akar->kanan, id);
    else {
        if (akar->kiri == NULL) {
            Anggota *temp = akar->kanan;
            free(akar);
            return temp;
        } else if (akar->kanan == NULL) {
            Anggota *temp = akar->kiri;
            free(akar);
            return temp;
        }
        Anggota *temp = akar->kanan;
        while (temp && temp->kiri != NULL)
            temp = temp->kiri;

        akar->id = temp->id;
        strcpy(akar->nama, temp->nama);
        akar->jumlah_pembelian = temp->jumlah_pembelian;
        akar->kanan = hapusAnggota(akar->kanan, temp->id);
    }
    return akar;
}

// nunjukin laporan keuangan gitu
void tampilkanLaporanKeuangan(Anggota *simpul) {
    static double totalKeuntungan = 0;
    if (simpul != NULL) {
        tampilkanLaporanKeuangan(simpul->kiri);
        totalKeuntungan += simpul->keuntunganTotal;
        tampilkanLaporanKeuangan(simpul->kanan);
    }
    if (simpul == akar) {
        printf("Total keuntungan perusahaan: %.2f\n", totalKeuntungan);
    }
}

int main() {
    int pilihan, id; 
    char referer_id[5], referal_tampil[10]="nihil";
    char nama[100], password[8], referalIn[10];
    

    clear();
    while (1) {
        printf("Kode Referal: %s\n", referal_tampil);
        printf("\n--- Menu Sistem MLM ---\n");
        printf("1. Daftar Anggota Baru\n");
        printf("2. Beli Paket\n"); 
        printf("3. Tampilkan Semua Anggota\n");
        printf("4. Laporan Keuangan\n");
        printf("5. Cari Anggota dengan Nama\n");
        printf("6. Hapus Anggota\n");
        printf("7. Update Anggota\n");
        printf("8. Tampilkan Struktur Pohon\n");
        printf("9. Tampilkan Statistik Keseluruhan\n");
        printf("10. Simpan dan Keluar\n");
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);
        clear();

        switch (pilihan) {
            case 1:
                printf("Masukkan ID, Kode Referal (0 jika tidak ada), dan Nama: ");
                scanf("%d %s %[^\n]s", &id, &referer_id, nama);
                printf("Masukkan password(maksimal 7 huruf): ");
                scanf("%s", password);
                printf("Buat Kode Referal(Maksimal 5 huruf): ");
                scanf("%s", referalIn);
                clear(); 
                akar = tambahAnggota(akar, id, nama, password, referer_id, referalIn);
                strcpy(referal_tampil,referalIn);
                break;
            case 2:
                printf("Masukkan ID Anggota: ");
                scanf("%d", &id);
                printf("Masukkan password: ");
                scanf("%s", &password);
                Anggota *ang = cariAnggota(akar, id);
                if (ang) {
                    prosesPembelian(ang, password);
                } else {
                    clear();
                    printf("Anggota tidak ditemukan!\n");
                }
                break;
            case 3:
                tampilkanSemuaAnggota(akar);
                break;
            case 4:
                tampilkanLaporanKeuangan(akar);
                break;
            case 5:
                printf("Masukkan nama anggota yang dicari: ");
                scanf(" %[^\n]s", nama);
                cariAnggotaDenganNama(akar, nama);
                break;
            case 6:
                printf("Masukkan ID Anggota yang akan dihapus: ");
                scanf("%d", &id);
                akar = hapusAnggota(akar, id);
                printf("Anggota dengan ID %d telah dihapus.\n", id);
                break;
            case 7:
                updateAnggota();
                break;
            case 8:
                printTree(akar, 0);
                break;
            case 9:
                displayStats();
                break;

            case 10:
                {
                    FILE *file0 = fopen("rekap-anggota.txt", "w"); 
                    if (file0 != NULL) {
                        simpanDataAnggotaKeFile(akar, file0); 
                        fclose(file0); 
                        printf("Data anggota telah disimpan ke 'rekap-anggota.txt'.\n");
                    } else {
                        printf("Gagal membuka file 'rekap-anggota.txt' untuk penyimpanan data.\n");
                    }
                    clear();
                    FILE *file1 = fopen("rekap-perusahaan.txt", "w"); 
                    if (file1 != NULL) {
                        simpanDataPerusahaanKeFile(akar, file1); 
                        fclose(file1); 
                        printf("Data perusahaan telah disimpan ke 'rekap-perusahaan.txt'.\n");
                    } else {
                        printf("Gagal membuka file 'rekap-perusahaan.txt' untuk penyimpanan data.\n");
                    }
                    clear();
                    return 0; 
                }
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
return 0;
}
