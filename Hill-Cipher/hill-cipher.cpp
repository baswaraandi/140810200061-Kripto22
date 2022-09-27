#include <iostream>
#include <limits>
#include <math.h>
#include <vector>

using matrix = std::vector<std::vector<int>>;

void menu();
void encryption(int);
void decryption(int);
void findKeyMatrix(int);

void getCofactor(matrix &, matrix &, int, int);
int determinant(matrix &, int);
void adjoint(matrix &, matrix &);
void displayMatrix(matrix);

int mod(int, int);

void ignoreLine();
void refreshUi(bool);
void errorMessage(std::string);

int main() {
  char yesNo;

  do {
    yesNo = ' ';

    std::cout << "\n.:: Hill Cipher ::.\n" << std::endl;
    menu();

    while (toupper(yesNo) != 'Y' && toupper(yesNo) != 'N') {
      std::cout << "\nJalankan lagi (Y/N)\t: ";
      std::cin >> yesNo;
      std::cin.ignore();
      refreshUi(false);
    }
  } while (toupper(yesNo) == 'Y');

  return 0;
}

void menu() {
  int m;

  std::cout << "Ordo matriks\t> ";
  std::cin >> m;

  int op;
  std::cout << "\n";
  std::cout << "1. Enkripsi\n";
  std::cout << "2. Dekripsi\n";
  std::cout << "3. Cari Kunci\n";
  std::cout << "Pilih operasi\t> ";
  std::cin >> op;

  switch (op) {
  case 1:
    encryption(m);
    break;
  case 2:
    decryption(m);
    break;
  case 3:
    findKeyMatrix(m);
    break;
  default:
    errorMessage("Invalid operation, please try again");
    break;
  }
}

void encryption(int m) {
  std::cout << "\n:: Enkripsi >\n";
  std::string text;
  int msg[m][1], hasil[m][1];
  matrix key;

  std::cout << "Pesan\t\t: ";
  std::cin >> text;

  std::cout << "Key\t:\n";
  int temp;
  for (int i = 0; i < m; i++) {
    std::vector<int> row;
    for (int j = 0; j < m; j++) {
      std::cin >> temp;
      row.push_back(temp);
    }
    key.push_back(row);
  }

  refreshUi(false);
  std::cout << "\n:: Enkripsi >\n";
  std::cout << "Pesan\t\t: " << text << std::endl;
  std::cout << "Key\t:\n";
  displayMatrix(key);

  std::string encryp = "";

  while (text.length() != 0) {
    // dipecah ke dalam submatriks berukuran m x 1
    for (int i = 0; i < m; i++) {
      if (isupper(text[i]))
        msg[i][0] = text[i] - 65;
      else
        msg[i][0] = text[i] - 97;
    }

    // dikalikan dengan invers dari key
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < 1; j++) {
        hasil[i][j] = 0;
        for (int k = 0; k < m; k++) {
          hasil[i][j] += key[i][k] * msg[k][j];
        }
        hasil[i][j] = hasil[i][j] % 26;
      }
    }

    // dikonversi ke hasil ciphertext
    for (int i = 0; i < m; i++) {
      if (isupper(text[i]))
        encryp += hasil[i][0] + 65;
      else
        encryp += hasil[i][0] + 97;
    }
    text.erase(0, m);
  }

  std::cout << "Ciphertext\t: " << encryp << std::endl;
}

void decryption(int m) {
  std::cout << "\n:: Dekripsi >\n";
  std::string text;
  int msg[m][1], hasil[m][1];
  matrix key;

  std::cout << "Pesan\t\t: ";
  std::cin >> text;

  std::cout << "Key Asli\t:\n";
  int temp;
  for (int i = 0; i < m; i++) {
    std::vector<int> row;
    for (int j = 0; j < m; j++) {
      std::cin >> temp;
      row.push_back(temp);
    }
    key.push_back(row);
  }

  refreshUi(false);
  std::cout << "\n:: Dekripsi >\n";
  std::cout << "Pesan\t\t: " << text << std::endl;
  std::cout << "Key Asli\t:\n";
  displayMatrix(key);

  // pencarian invers dari key
  matrix adj(key.size(), std::vector<int>(key.size()));

  int det = determinant(key, m);
  adjoint(key, adj);

  int det_inv = 0;
  int flag = 0;

  for (int i = 0; i < 26; i++) {
    flag = mod((det * i), 26);

    if (flag == 1) {
      det_inv = i;
    }
  }

  matrix i_mtx(key.size(), std::vector<int>(key.size()));

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      i_mtx[i][j] = mod((adj[i][j] * det_inv), 26);
    }
  }

  std::cout << "\nInvers dari Key :\n";
  displayMatrix(i_mtx);

  std::string decryp = "";

  while (text.length() != 0) {
    // dipecah ke dalam submatriks berukuran m x 1
    for (int i = 0; i < m; i++) {
      if (isupper(text[i]))
        msg[i][0] = text[i] - 65;
      else
        msg[i][0] = text[i] - 97;
    }

    // dikalikan dengan invers dari key
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < 1; j++) {
        hasil[i][j] = 0;
        for (int k = 0; k < m; k++) {
          hasil[i][j] += i_mtx[i][k] * msg[k][j];
        }
        hasil[i][j] = hasil[i][j] % 26;
      }
    }

    // dikonversi ke hasil plaintext
    for (int i = 0; i < m; i++) {
      if (isupper(text[i]))
        decryp += hasil[i][0] + 65;
      else
        decryp += hasil[i][0] + 97;
    }
    text.erase(0, m);
  }

  std::cout << "\nPlaintext\t: " << decryp << std::endl;
}

void findKeyMatrix(int m) {
  std::cout << "\n:: Cari Kunci >\n";
  std::string plain, cipher;
  int pln[m][1], cpr[m][1];
  matrix key(m, std::vector<int>(m)), c(m, std::vector<int>(m)),
      p(m, std::vector<int>(m)), adj(p.size(), std::vector<int>(p.size())),
      p_inv(p.size(), std::vector<int>(p.size()));

  std::cout << "Plaintext\t: ";
  std::cin >> plain;
  std::cout << "Ciphertext\t: ";
  std::cin >> cipher;

  if (plain.length() != cipher.length()) {
    errorMessage("Panjang Plaintext dan Ciphertext Harus Sama!");
    return;
  }

  // pengelompokkan ke dalam matriks C dan P
  int k = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      if (isupper(plain[k])) {
        p[j][i] = plain[k] - 65;
      } else {
        p[j][i] = plain[k] - 97;
      }
      k++;
    }
  }

  k = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      if (isupper(cipher[k])) {
        c[j][i] = cipher[k] - 65;
      } else {
        c[j][i] = cipher[k] - 97;
      }
      k++;
    }
  }

  // P invers
  int det = determinant(p, m);
  adjoint(p, adj);

  int det_inv = 0;
  int flag = 0;

  for (int i = 0; i < 26; i++) {
    flag = mod((det * i), 26);

    if (flag == 1) {
      det_inv = i;
    }
  }

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      p_inv[i][j] = mod((adj[i][j] * det_inv), 26);
    }
  }

  // C * P invers
  for (int i = 0; i < c.size(); i++) {
    for (int j = 0; j < p_inv.size(); j++) {
      for (int k = 0; k < c.size(); k++) {
        key[i][j] = mod((key[i][j] + c[i][k] * p_inv[k][j]), 26);
      }
    }
  }

  std::cout << "\nMatriks Kunci\t:\n";
  displayMatrix(key);
}

void getCofactor(matrix &key, matrix &temp, int p, int q) {
  int i = 0, j = 0;
  for (int r = 0; r < key.size(); r++) {
    for (int c = 0; c < key.size(); c++) {
      if (r != p && c != q) {
        temp[i][j++] = key[r][c];
      }
      if (j == key.size() - 1) {
        j = 0;
        i++;
      }
    }
  }
}

int determinant(matrix &key, int n) {
  if (n == 1) {
    return key[0][0];
  }
  int det = 0;
  matrix temp(3, std::vector<int>(3));
  int sign = 1;

  for (int i = 0; i < key.size(); i++) {
    getCofactor(key, temp, 0, i);
    det += sign * key[0][i] * determinant(temp, n - 1);
    sign = -sign;
  }
  return det;
}

void adjoint(matrix &key, matrix &adj) {
  if (key.size() == 1) {
    adj[0][0] = 1;
    return;
  }
  int sign = 1;
  matrix temp(key.size(), std::vector<int>(key.size()));
  for (int i = 0; i < key.size(); i++) {
    for (int j = 0; j < key.size(); j++) {
      getCofactor(key, temp, i, j);
      sign = ((i + j) % 2 == 0) ? 1 : -1;
      adj[j][i] = (sign) * (determinant(temp, key.size() - 1));
    }
  }
}

void displayMatrix(matrix mtx) {
  for (int i = 0; i < mtx.size(); i++) {
    for (int j = 0; j < mtx.size(); j++) {
      std::cout << mtx[i][j] << "\t";
    }
    std::cout << "\n";
  }
}

int mod(int a, int b) { return (a % b + b) % b; };

void ignoreLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void refreshUi(bool wait) {
  if (wait) {
    std::cout << "Press any b to continue...";
    std::cin.ignore();
    std::cin.get();
  }
  std::cout << "\033[2J\033[1;1H";
}

void errorMessage(std::string s) {
  std::cout << "\n//ERROR\n";
  std::cout << s.c_str() << '\n';
}