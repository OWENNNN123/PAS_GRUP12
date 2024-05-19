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
    struct Anggota* sponsor;  // Pointer to the member who referred
} Anggota;

Anggota *akar = NULL;  // Root of the Binary Search Tree

// Function Declarations
void muatData();
void simpanData(Anggota* node, FILE* file);
void beliPaket(int id);
void printTree(Anggota* node, int space);
void displayStats();
void updateAnggota(int id);
int askForId();
void cetakAnggota(Anggota* node);
void laporanKeuangan(Anggota* node);
Anggota* cariAnggota(Anggota* node, int id);
Anggota* cariAnggotaDenganNama(Anggota* node, char* nama);
Anggota* minValueNode(Anggota* node);

// Function to create a new member
Anggota* buatAnggota(int id, char* nama, Anggota* sponsor) {
    Anggota* baru = (Anggota*)malloc(sizeof(Anggota));
    if (!baru) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    baru->id = id;
    strcpy(baru->nama, nama);
    baru->jumlahBeli = 0;
    baru->komisi = 0;
    baru->kiri = baru->kanan = NULL;
    baru->sponsor = sponsor;
    return baru;
}

// Function to add a member to the BST
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

// Function to remove a member from the BST
Anggota* hapusAnggota(Anggota* root, int id) {
    if (root == NULL) return NULL;
    if (id < root->id)
        root->kiri = hapusAnggota(root->kiri, id);
    else if (id > root->id)
        root->kanan = hapusAnggota(root->kanan, id);
    else {
        if (root->kiri == NULL) {
            Anggota* temp = root->kanan;
            free(root);
            return temp;
        } else if (root->kanan == NULL) {
            Anggota* temp = root->kiri;
            free(root);
            return temp;
        }
        Anggota* temp = minValueNode(root->kanan);
        root->id = temp->id;
        strcpy(root->nama, temp->nama);
        root->jumlahBeli = temp->jumlahBeli;
        root->komisi = temp->komisi;
        root->sponsor = temp->sponsor;
        root->kanan = hapusAnggota(root->kanan, temp->id);
    }
    return root;
}

// Function to find the minimum value node
Anggota* minValueNode(Anggota* node) {
    Anggota* current = node;
    while (current && current->kiri != NULL)
        current = current->kiri;
    return current;
}

// Function to search a member by ID
Anggota* cariAnggota(Anggota* node, int id) {
    if (node == NULL || node->id == id)
        return node;
    if (id < node->id)
        return cariAnggota(node->kiri, id);
    else
        return cariAnggota(node->kanan, id);
}

// Function to update member data
void updateAnggota(int id) {
    Anggota* anggota = cariAnggota(akar, id);
    if (anggota == NULL) {
        printf("Anggota tidak ditemukan.\n");
        return;
    }
    printf("Update nama (current: %s): ", anggota->nama);
    scanf("%s", anggota->nama);
    printf("Update jumlah beli (current: %d): ", anggota->jumlahBeli);
    scanf("%d", &anggota->jumlahBeli);
}

// Function to display the BST structure visually
void printTree(Anggota* node, int space) {
    if (node == NULL)
        return;

    space += 10;
    printTree(node->kanan, space);
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d (%s)\n", node->id, node->nama);
    printTree(node->kiri, space);
}

// Function to load data from a file
void muatData() {
    FILE* file = fopen("anggota.txt", "r");
    if (file == NULL) return;
    int id, jumlahBeli;
    float komisi;
    char nama[50];
    while(fscanf(file, "%d %s %d %f", &id, nama, &jumlahBeli, &komisi) != EOF) {
        Anggota* sponsor = NULL;
        akar = tambahAnggota(akar, id, nama, sponsor);
        Anggota* anggota = cariAnggota(akar, id);
        anggota->jumlahBeli = jumlahBeli;
        anggota->komisi = komisi;
    }
    fclose(file);
}

// Function to display statistics about the members
void displayStats() {
    int totalMembers = 0;
    float totalKomisi = 0;
    int totalBeli = 0;

    void calculateStats(Anggota* node) {
        if (node == NULL) return;
        totalMembers++;
        totalKomisi += node->komisi;
        totalBeli += node->jumlahBeli;
        calculateStats(node->kiri);
        calculateStats(node->kanan);
    }

    calculateStats(akar);
    printf("Total Anggota: %d\n", totalMembers);
    printf("Total Komisi: %.2f\n", totalKomisi);
    printf("Rata-rata Jumlah Beli: %.2f\n", totalMembers ? (float)totalBeli / totalMembers : 0);
}

// Function to save data to a file
void simpanData(Anggota* node, FILE* file) {
    if (node == NULL) return;
    fprintf(file, "%d %s %d %.2f\n", node->id, node->nama, node->jumlahBeli, node->komisi);
    if (node->kiri) simpanData(node->kiri, file);
    if (node->kanan) simpanData(node->kanan, file);
}

// Function to search a member by name (Assumed function needed to be implemented)
Anggota* cariAnggotaDenganNama(Anggota* node, char* nama) {
    if (node == NULL) return NULL;
    if (strcmp(node->nama, nama) == 0) return node;
    Anggota* found = cariAnggotaDenganNama(node->kiri, nama);
    if (found == NULL) found = cariAnggotaDenganNama(node->kanan, nama);
    return found;
}

// Menu to handle user input
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
        printf("4. Laporan Keuangan\n");
        printf("5. Cari Anggota dengan Nama\n");
        printf("6. Hapus Anggota\n");
        printf("7. Update Anggota\n");
        printf("8. Tampilkan Struktur Pohon\n");
        printf("9. Tampilkan Statistik Keseluruhan\n");
        printf("10. Simpan dan Keluar\n");
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
                laporanKeuangan(akar);
                break;
            case 5:
                printf("Masukkan nama anggota yang ingin dicari: ");
                scanf("%s", nama);
                Anggota* hasil = cariAnggotaDenganNama(akar, nama);
                if (hasil != NULL) {
                    printf("Anggota ditemukan: ID %d, Nama %s, Jumlah Beli %d, Komisi %.2f\n",
                           hasil->id, hasil->nama, hasil->jumlahBeli, hasil->komisi);
                } else {
                    printf("Tidak ada anggota dengan nama tersebut.\n");
                }
                break;
            case 6:
                printf("Masukkan ID anggota yang akan dihapus: ");
                scanf("%d", &id);
                akar = hapusAnggota(akar, id);
                printf("Anggota telah dihapus.\n");
                break;
            case 7:
                printf("Masukkan ID anggota yang akan diupdate: ");
                id = askForId();
                updateAnggota(id);
                break;
            case 8:
                printTree(akar, 0);
                break;
            case 9:
                displayStats();
                break;
            case 10:
                printf("Menyimpan data...\n");
                FILE* file = fopen("anggota.txt", "w");
                if (file != NULL) {
                    simpanData(akar, file);
                    fclose(file);
                    printf("Data telah disimpan.\n");
                } else {
                    printf("Gagal menyimpan data.\n");
                }
                printf("Terima kasih telah menggunakan program MLM kami.\n");
                return;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 10);
}
int main() {
    menu();
    return 0;
}