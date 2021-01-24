#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
using namespace std;

namespace little_endian_io
{
  template <typename Word>
  std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
  {
    for (; size; --size, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
  }
}
using namespace little_endian_io;

map<char, string> alph_m() {
    map<char, string> dico = {{'a', "io"},
                              {'b', "oiii"},
                              {'c', "oioi"},
                              {'d', "oii"},
                              {'e', "i"},
                              {'f', "iioi"},
                              {'g', "ooi"},
                              {'h', "iiii"},
                              {'i', "ii"},
                              {'j', "iooo"},
                              {'k', "ioi"},
                              {'l', "ioii"},
                              {'m', "oo"},
                              {'n', "oi"},
                              {'o', "ooo"},
                              {'p', "iooi"},
                              {'q', "ooio"},
                              {'r', "oio"},
                              {'s', "iii"},
                              {'t', "o"},
                              {'u', "iio"},
                              {'v', "iiio"},
                              {'w', "ioo"},
                              {'x', "oiio"},
                              {'y', "oioo"},
                              {'z', "ooii"}
                              };
    return dico;
}

map<string, char> m_alph() {
    map<string, char> dico = {{"io", 'a'},
                              {"oiii", 'b'},
                              {"oioi", 'c'},
                              {"oii", 'd'},
                              {"i", 'e'},
                              {"iioi", 'f'},
                              {"ooi", 'g'},
                              {"iiii", 'h'},
                              {"ii", 'i'},
                              {"iooo", 'j'},
                              {"ioi", 'k'},
                              {"ioii", 'l'},
                              {"oo", 'm'},
                              {"oi", 'n'},
                              {"ooo", 'o'},
                              {"iooi", 'p'},
                              {"ooio", 'q'},
                              {"oio", 'r'},
                              {"iii", 's'},
                              {"o", 't'},
                              {"iio", 'u'},
                              {"iiio", 'v'},
                              {"ioo", 'w'},
                              {"oiio", 'x'},
                              {"oioo", 'y'},
                              {"ooii", 'z'}
                              };
    return dico;
}


void francais_vers_audio(string mot){
    ofstream f( "mot_morse.wav", ios::binary );

    // Ecriture de l'en-tête du fichier
    f << "RIFF----WAVEfmt ";     // (partie du fichier à remplir par la suite)
    write_word( f,     16, 4 );  // no extension data
    write_word( f,      1, 2 );  // PCM - integer samples
    write_word( f,      2, 2 );  // two channels (stereo file)
    write_word( f,  44100, 4 );  // samples per second (Hz)
    write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
    write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
    write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)

    // Ecriture de l’en-tête de morceau de données
    size_t data_chunk_pos = f.tellp();
    f << "data----";  // (partie du fichier à remplir par la suite)
  
    // Ecriture de la partie qui sera audible
    constexpr double two_pi = 6.283185307179586476925286766559;
    constexpr double max_amplitude = 32760;  // "volume"
    constexpr double son_vide = 0;

    double hz        = 44100;    // échantillons par seconde
    double frequency = 261.626;  // do
    double son_c = 0.5; //durée d'un son court
    double son_l = son_c*3; //durée d'un son court
    double pause_lettre = son_c*2;
    double pause_symb = son_c;
    double pause_mot = son_c*4;
    int N;  // Nombre total d'échantillons
    double amplitude;
    double value;

    map<char, string>  liste = alph_m();
    char lettre;
    string code;

    //on parcourt mot et on écrit dans le fichier .wav au fur et à mesure
    for (int i=0; i< mot.size(); i++){
        lettre = mot[i];
        if (lettre == '.'){
            //lettre est un point c'est-à-dire un espace entre deux mots, donc on ajoute un silence
            N = hz * pause_mot;
            for (int n = 0; n < N; n++) {
                amplitude = (double)n / N * son_vide;
                value = sin( (two_pi * n * frequency) / hz );
                write_word( f, (int)(                 amplitude  * value), 2 );
                write_word( f, (int)((son_vide - amplitude) * value), 2 );
            };
        }
        else {
            code = liste[lettre];
            int l = code.size();
            //si c'est une lettre, on parcourt le code morse qui lui correspond
            for (int j=0; j< l; j++){
                if (code[j] == 'i') {
                    N = hz * son_c;
                    for (int n = 0; n < N; n++) {
                        amplitude = (double)n / N * max_amplitude;
                        value = sin( (two_pi * n * frequency) / hz );
                        write_word( f, (int)(                 amplitude  * value), 2 );
                        write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
                    };
                    N = hz * pause_symb;
                    for (int n = 0; n < N; n++) {
                        amplitude = (double)n / N * son_vide;
                        value = sin( (two_pi * n * frequency) / hz );
                        write_word( f, (int)(                 amplitude  * value), 2 );
                        write_word( f, (int)((son_vide - amplitude) * value), 2 );
                    };
                }
                else{
                    N = hz * son_l;
                    for (int n = 0; n < N; n++){
                        amplitude = (double)n / N * max_amplitude;
                        value = sin( (two_pi * n * frequency) / hz );
                        write_word( f, (int)(                 amplitude  * value), 2 );
                        write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
                    };
                    N = hz * pause_symb;
                        for (int n = 0; n < N; n++) {
                            amplitude = (double)n / N * son_vide;
                            value = sin( (two_pi * n * frequency) / hz );
                            write_word( f, (int)(                 amplitude  * value), 2 );
                            write_word( f, (int)((son_vide - amplitude) * value), 2 );
                        };
                };
            };
            N = hz * pause_lettre;
            for (int n = 0; n < N; n++) {
                amplitude = (double)n / N * son_vide;
                value = sin( (two_pi * n * frequency) / hz );
                write_word( f, (int)(                 amplitude  * value), 2 );
                write_word( f, (int)((son_vide - amplitude) * value), 2 );
            };
        };
    }
  
    // Nous aurons besoin de la taille finale du fichier pour fixer les tailles de morceaux ci-dessus
    size_t file_length = f.tellp();

    // On fixe l’en-tête du morceau de données pour contenir la taille des données
    f.seekp( data_chunk_pos + 4 );
    write_word( f, file_length - data_chunk_pos + 8 );

    // On fixe l’en-tête du fichier pour contenir la taille appropriée du morceau RIFF, qui est (taille du fichier - 8) octets
    f.seekp( 0 + 4 );
    write_word( f, file_length - 8, 4 );
}


string audio_vers_morse(string file_name){
    string morse=""; //on va traduire avec des 'i', des 'o', des '/' et des ' ' le fichier audio en morse
    ifstream f;
    f.open(file_name, ios::binary);

    if (f.is_open()){
        // On récupère la taille du fichier
        f.ignore(40);
        uint32_t taille;
        f.read(reinterpret_cast<char *>(&taille), sizeof (taille));

        // On fait une première lecture du fichier pour obtenir la taille du tableau qui va stocker la durée des sons et des silences
        int taille_tableau = 0; // Taille initialisée à 0

        uint32_t var = 1; // Variable qui récupère la valeur du sample
        bool boucle = true; // variable qui sert à continuer les boucles qui lisent les sons si un sample vaut 0

        for (; f.tellg() < taille ;){
            taille_tableau++;
            for (; f.tellg() < taille and boucle ;){
                f.read(reinterpret_cast<char *>(&var), sizeof (var));
                if (var == 0 and f.tellg() < taille) // Si un échantillon est nul on regarde si le suivant aussi est nul, si c'est le cas, boucle devient false car on passe à un silence
                {
                    f.read(reinterpret_cast<char *>(&var), sizeof (var));
                    if (var == 0){
                        boucle = false;
                    }
                }
            }
            // On vient de lire un son, par la suite on va lire un silence
            boucle = true;
            for (; f.tellg() < taille and var == 0 ;){
                f.read(reinterpret_cast<char *>(&var), sizeof (var));
            }
        }

        // Tableaux à remplir des durées (en nombre de sample) des sons et des silences
        int uns[taille_tableau] = {0};
        int zeros[taille_tableau] = {0};

        // On parcours à nouveau le fichier pour remplir les tableaux avec la durée des 1 et des 0
        f.seekg(40);

        var = 1;
        boucle = true;

        int c = 0; //Compteur du nombre de sample

        //Variables qui stockent la durée maximale et minimale d'un son et d'un silence
        int min0 = taille;
        int min1 = taille;
        int max0 = 0;
        int max1 = 0;

        for (int i = 0; f.tellg() < taille ; i++){
            // Lecture d'un son
            for (; f.tellg() < taille and boucle ;){
                f.read(reinterpret_cast<char *>(&var), sizeof (var));
                c++;
                if (var == 0 and f.tellg() < taille){
                    f.read(reinterpret_cast<char *>(&var), sizeof (var));
                    c++;
                    if (var == 0){
                        boucle = false;
                    }
                }
            }
            boucle = true;
            uns[i] = c;

            max1 = max(c, max1);
            min1 = min(c, min1);

            c = 0;
            // Lecture d'un silence
            for (int i = 0; f.tellg() < taille and var == 0 ; i++){
                f.read(reinterpret_cast<char *>(&var), sizeof (var));
                c++;
            }

            zeros[i] = c;

            max0 = max(c, max0);
            min0 = min(c, min0);

            c = 0;
        }

        // On parcours les tableaux pour déterminer les caractères en morse           
        for(int i = 0; i < taille_tableau ; i++){
            // On est parti du principe que tous les fichiers commencent par un son
            int un = uns[i];
            int zero = zeros[i];

            // On se donne une marge d'erreur
            if (un < 1.1*max1 and un > 0.9*max1){
                morse += (string)"o";
            }
            else //if (un < 1.1*min1 and un > 0.9*min1)
            {
                morse += (string)"i";
            }

            if (zero < 1.05*min0 and zero > 0.95*min0){} // C'est juste un espace entre caractère morse
            else {if (zero < 1.05*max0 and zero > 0.95*max0)
            {
                morse += (string)" /";
            }
                else{
                    morse += (string)" "; // Si ce n'est ni un espace entre caractères morses ni un espace entre mots, c'est un espace entre lettres
                }
            }
        }
        
        f.close();
    }
    return morse;
}


void morse_vers_francais(string nom_fichier){
    map<string, char> liste = m_alph();
    string morse = audio_vers_morse(nom_fichier);
    string texte = "";
    int parcourt = 0;

    while (parcourt < morse.size()){
        if (morse[parcourt] == '/'){ //c'est un espace entre mots
            texte+= (string) " ";
            parcourt++;
        }
        else {
            string symb = "";
            while (parcourt < morse.size() and morse[parcourt]!=' '){ //on cherche à obtenir le plus grand symbole morse (avant de passer à une autre lettre)
                symb+= morse[parcourt];
                parcourt++;
            };
            texte+= (string) &(liste[symb]);
            parcourt++;
        }
    };
    cout << texte << endl;
};

int main(){
    char debut;
    cout << "Entrez E pour encoder, ou D pour décoder" << endl;
    cin >> debut;
    if (debut == 'E'){
        cout << "Entrez le texte en mettant uniquement des caractères minuscules et des points entre chaque mot. Exemple, la.vie.est.belle" << endl;
        string texte = "";
        cin >> texte;
        francais_vers_audio(texte);
    }
    else {
        cout << "Entrez le nom du fichier. Exemple, audio.wav" << endl;
        string nom_fichier;
        cin >> nom_fichier;
        morse_vers_francais(nom_fichier);
    };
    return 0;
}