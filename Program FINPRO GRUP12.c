#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HARGA_PASAR 100

typedef struct Anggota {
    char nama[50];
    int id;
    int jumlahBeli;
    float komisi;
    struct Anggota* kiri;
    struct Anggota* kanan;
    struct Anggota* sponsor;  // Pointer ke anggota yang mereferensikan
} Anggota;

Anggota *akar = NULL;  // Akar dari Binary Search Tree

// Fungsi untuk membuat anggota baru
Anggota* buatAnggota(int id, char* nama, Anggota* sponsor) {
    Anggota* baru = (Anggota*)malloc(sizeof(Anggota));
    baru->id = id;
    strcpy(baru->nama, nama);
    baru->jumlahBeli = 0;
    baru->komisi = 0;
    baru->kiri = baru->kanan = NULL;
    baru->sponsor = sponsor;
    return baru;
}

// Fungsi untuk menambahkan anggota baru ke BST
Anggota* tambahAnggota(Anggota* node, int id, char* nama, Anggota* sponsor) {
    if (node == NULL) {
        return buatAnggota(id, nama, sponsor);
    }

    if (id < node->id)
        node->kiri = tambahAnggota(node->kiri, id, nama, sponsor);
    else if (id > node->id)
        node->kanan = tambahAnggota(node->kanan, id, nama, sponsor);

    return node;
}

// Fungsi untuk mencari anggota berdasarkan ID
Anggota* cariAnggota(Anggota* node, int id) {
    if (node == NULL || node->id == id)
        return node;

    if (id < node->id)
        return cariAnggota(node->kiri, id);
    else
        return cariAnggota(node->kanan, id);
}

// Fungsi untuk menghitung harga pembelian paket
int hitungHarga(int jumlahBeli) {
    if (jumlahBeli == 0)
        return 2 * HARGA_PASAR;
    else if (jumlahBeli == 1)
        return HARGA_PASAR;
    else
        return 0.8 * HARGA_PASAR;
}

// Fungsi untuk melakukan pembelian paket
void beliPaket(int id) {
    Anggota* anggota = cariAnggota(akar, id);
    if (anggota == NULL) {
        printf("Anggota tidak ditemukan.\n");
        return;
    }

    int harga = hitungHarga(anggota->jumlahBeli);
    printf("Pembelian paket berhasil. Harga paket: %d dari harga pasar.\n", harga);
    anggota->jumlahBeli++;
    if (anggota->sponsor != NULL) {
        // Komisi untuk sponsor
        float komisi = 0.5 * harga;
        anggota->sponsor->komisi += komisi;
        printf("Komisi %.2f diberikan kepada sponsor %s.\n", komisi, anggota->sponsor->nama);
    }
}

// Fungsi untuk mencetak semua anggota secara in-order
void cetakAnggota(Anggota* node) {
    if (node != NULL) {
        cetakAnggota(node->kiri);
        printf("ID: %d, Nama: %s, Jumlah Beli: %d, Komisi: %.2f\n", node->id, node->nama, node->jumlahBeli, node->komisi);
        cetakAnggota(node->kanan);
    }
}

// Fungsi untuk menyimpan data ke file
void simpanData(Anggota* node, FILE* file) {
    if (node == NULL) return;
    fprintf(file, "%d %s %d %.2f\n", node->id, node->nama, node->jumlahBeli, node->komisi);
    simpanData(node->kiri, file);
    simpanData(node->kanan, file);
}

// Fungsi untuk memuat data dari file
void muatData() {
    FILE* file = fopen("anggota.txt", "r");
    if (file == NULL) return;
    int id, jumlahBeli;
    float komisi;
    char nama[50];
    while(fscanf(file, "%d %s %d %f", &id, nama, &jumlahBeli, &komisi) != EOF) {
        Anggota* sponsor = NULL;  // Assumed no sponsor information stored
        akar = tambahAnggota(akar, id, nama, sponsor);
        Anggota* anggota = cariAnggota(akar, id);
        anggota->jumlahBeli = jumlahBeli;
        anggota->komisi = komisi;
    }
    fclose(file);
}

// Fungsi untuk menampilkan menu dan mengolah pilihan pengguna
void menu() {
    int pilihan, id;
    char nama[50];
    int sponsor_id;
    Anggota* sponsor;

    printf("Memuat data anggota...\n");
    muatData();

    do {
        printf("\nMenu MLM:\n");
        printf("1. Tambah Anggota\n");
        printf("2. Beli Paket Dagang\n");
        printf("3. Tampilkan Semua Anggota\n");
        printf("4. Simpan dan Keluar\n");
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);

        switch(pilihan) {
            case 1:
                printf("Masukkan ID anggota baru: ");
                scanf("%d", &id);
                printf("Masukkan nama anggota baru: ");
                scanf("%s", nama);
                printf("Masukkan ID sponsor (0 jika tidak ada): ");
                scanf("%d", &sponsor_id);
                sponsor = (sponsor_id == 0) ? NULL : cariAnggota(akar, sponsor_id);
                akar = tambahAnggota(akar, id, nama, sponsor);
                break;
            case 2:
                printf("Masukkan ID anggota yang akan membeli paket: ");
                scanf("%d", &id);
                beliPaket(id);
                break;
            case 3:
                cetakAnggota(akar);
                break;
            case 4:
                printf("Menyimpan data...\n");
                FILE* file = fopen("anggota.txt", "w");
                simpanData(akar, file);
                fclose(file);
                printf("Terima kasih telah menggunakan program MLM kami.\n");
                return;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 4);
}

int main() {
    menu();
    return 0;
}
