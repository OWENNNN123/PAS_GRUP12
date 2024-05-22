#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define HARGA_DASAR 100 

typedef struct Anggota {
    int id;
    char nama[100];
    int jumlah_pembelian;
    double keuntunganTotal;
    struct Anggota *kiri, *kanan;
    char referer_id[5];
    char referer_code[5];
    int komisi;
} Anggota;

Anggota *akar = NULL;

//  fungsi buat nyari anggota pake id
Anggota* cariAnggota(Anggota *akar, int id) {
    if (akar == NULL || akar->id == id)
        return akar;
    if (id < akar->id)
        return cariAnggota(akar->kiri, id);
    return cariAnggota(akar->kanan, id);
}

// sama tpi pake nama
void cariAnggotaDenganNama(Anggota *akar, char *nama) {
    if (akar != NULL) {
        cariAnggotaDenganNama(akar->kiri, nama);
        if (strcmp(akar->nama, nama) == 0) {
            printf("ID: %d, Nama: %s, Pembelian: %d, Komisi %d\n", akar->id, akar->nama, akar->jumlah_pembelian, akar->komisi);
        }
        cariAnggotaDenganNama(akar->kanan, nama);
    }
}
// sama tpi pake nama
void komisiReferal(Anggota *akar, char *referer_id) {
    if (akar != NULL) {
        komisiReferal(akar->kiri, referer_id);
        if (strcmp(akar->referer_code, referer_id) == 0) {
            akar->komisi+=2600;
        }
        komisiReferal(akar->kanan, referer_id);

    }
}

// buat update data anggota yg udh ditambah
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

//  fungsi buat print jadi kek pohon gitu
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
            totalAnggota++;
            totalPembelian += node->jumlah_pembelian;
            totalKeuntungan += node->keuntunganTotal;
            aggregateStats(node->kiri);
            aggregateStats(node->kanan);
        }
    }

    aggregateStats(akar);
    printf("Total Anggota: %d\n", totalAnggota);
    printf("Total Pembelian: %d\n", totalPembelian);
    printf("Total Keuntungan: %.2f\n", totalKeuntungan);
}



// tampilin semua anggota
void tampilkanSemuaAnggota(Anggota *node) {
    if (node != NULL) {
        tampilkanSemuaAnggota(node->kiri);
        printf("ID: %d, Nama: %s, Jumlah Pembelian: %d, Keuntungan Total: %.2f, Komisi Total: %.2f\n", node->id, node->nama, node->jumlah_pembelian, node->keuntunganTotal, node->referer_code, node->komisi);
        tampilkanSemuaAnggota(node->kanan);
    }
}


// nambah anggota baru
Anggota* tambahAnggota(Anggota *simpul, int id, char *nama, char *referer_id) {
    int i, komisi=0;
      komisiReferal(simpul->kanan, referer_id);
    if (simpul == NULL) {
        Anggota *simpulBaru = (Anggota*)malloc(sizeof(Anggota));
        simpulBaru->id = id;
        strcpy(simpulBaru->nama, nama);
        simpulBaru->jumlah_pembelian = 0;
        simpulBaru->keuntunganTotal = 0;
        simpulBaru->kiri = simpulBaru->kanan = NULL;
        strcpy(simpulBaru->referer_id, referer_id);
        return simpulBaru;
    }
    if (id < simpul->id)
        simpul->kiri = tambahAnggota(simpul->kiri, id, nama, referer_id);
    else if (id > simpul->id)
        simpul->kanan = tambahAnggota(simpul->kanan, id, nama, referer_id);
    return simpul;
}

// fungsi buat di save ke txt file
void simpanDataKeFile(Anggota *node, FILE *file) {
    if (node != NULL) {
        simpanDataKeFile(node->kiri, file);
        fprintf(file, "ID: %d, Nama: %s, Jumlah Pembelian: %d, Keuntungan Total: %.2f\n", 
                node->id, node->nama, node->jumlah_pembelian, node->keuntunganTotal);
        simpanDataKeFile(node->kanan, file);
    }
}




// buat proses beli
void prosesPembelian(Anggota *ang) {
    double hargaPembelian, komisi, keuntunganPerusahaan;
    if (ang->jumlah_pembelian == 0) {
        hargaPembelian = 2.0 * HARGA_DASAR;
        keuntunganPerusahaan = 0.55 * hargaPembelian;
        komisi = 0.5 * hargaPembelian;
    } else if (ang->jumlah_pembelian == 1) {
        hargaPembelian = HARGA_DASAR;
        keuntunganPerusahaan = 0.25 * hargaPembelian;
        komisi = 0; // Tidak ada komisi untuk pembelian kedua
    } else {
        hargaPembelian = 0.8 * HARGA_DASAR;
        keuntunganPerusahaan = 0.05 * hargaPembelian;
        komisi = 0; // Tidak ada komisi untuk pembelian ketiga dan seterusnya
    }

    printf("Harga Pembelian: %.2f\n", hargaPembelian);
    printf("Keuntungan Perusahaan: %.2f\n", keuntunganPerusahaan);
    ang->jumlah_pembelian++;
    ang->keuntunganTotal += keuntunganPerusahaan;
}

// untuk menghapus anggota
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
    char referer_id[5];
    char nama[100];

    while (1) {
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

        switch (pilihan) {
            case 1:
                printf("Masukkan ID, Nama, dan ID Referer (0 jika tidak ada): ");
                scanf("%d %[^\n]s %d", &id, nama, &referer_id);
                akar = tambahAnggota(akar, id, nama, referer_id);
                break;
            case 2:
                printf("Masukkan ID Anggota: ");
                scanf("%d", &id);
                Anggota *ang = cariAnggota(akar, id);
                if (ang) {
                    prosesPembelian(ang);
                } else {
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
                    FILE *file = fopen("rekap.txt", "w"); 
                    if (file != NULL) {
                        simpanDataKeFile(akar, file); 
                        fclose(file); 
                        printf("Data telah disimpan ke 'rekap.txt'.\n");
                    } else {
                        printf("Gagal membuka file 'rekap.txt' untuk penyimpanan data.\n");
                    }
                    return 0; 
                }
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
return 0;
}
