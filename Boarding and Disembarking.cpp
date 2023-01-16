#include <bits/stdc++.h>
#include <vector>

#define FASTIO ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define ll long long int
#define FOR(i, a, b) for (int i = a; i < b; i++)
#define FORR(i, a, b) for (int i = b-1; i > a-1; i--)
#define endl "\n"

using namespace std;

int l, p;
int up[500];
int down[500];
int winup[500][500];
int windown[500][500];
int hallP[500];
int hallM[500];
int hallstateP[500];
int hallstateM[500];
int A=1, B=5, X=2;

struct seat{
    int col;
    int row;
};

int *hall(int id){
    if(id>=0) return &hallP[id];
    if(id<0) return &hallM[-id];
}

int *hallstate(int id){
    if(id>=0) return &hallstateP[id];
    if(id<0) return &hallstateM[-id];
}

int* Permutation(int Size)
{
    int* DataArry = new int[Size];
    srand((unsigned int)(time(NULL)+rand()+rand()*rand()));
    bool Used[1000] = {0};
    for(int a=0;a<Size;a++)
    {
        srand((unsigned int)(time(NULL)+rand()*100000+rand()*rand()*100000));
        int Ran = rand()%Size;
        for(int I=Ran;;I++)
        {
            if(I==Size) I=0;
            if(Used[I]) continue;
            DataArry[a]=I;
            Used[I]=1;
            break;
        }
    }
    return DataArry;
}

seat idToseat(int id){
    int nowid = 0;
    seat s;

    for (int i = 0; i < l; i++){
        if (id - nowid >= down[i] + up[i]){
            nowid += down[i] + up[i];
            continue;
        }

        s.row = i;
        if (id - nowid >= down[i]){
            s.col = id - nowid - down[i] + 1;
        } else {
            s.col = id - nowid - down[i];
        }
        break;
    }

    return s;
}

class Passenger{
public:
    seat seat;
    int state;
    int col;
    int row;
    int tick;

    Passenger(){
        return;
    }

    void setinfo(int id, int i){
        this->seat = idToseat(id);
        this->state = 0;
        this->col = 0;
        this->row = -i-1;
        this->tick = A;
    }

    void op(){
        // printf("tick = %d, State = %d, Seat = %d, col = %d, row=%d\n", this->tick, this->state, this->seat.col, this->col, this->row);
        if (this->state == 0){
                this->tick--;
                if (this->tick < 0) this->tick = 0;

                if (*hallstate(this->row+1) == 1) {
                    this->tick = A+1;
                }

                if (*hall(this->row+1) != 0){
                    return;
                }

                if (this->tick == 0){
                    *hall(this->row) = *hall(this->row) - 1;
                    this->row++;
                    *hall(this->row) = *hall(this->row) + 1;

                    if (this->seat.row != this->row) {
                        this->tick = A;
                    } else {
                        int M = 0;
                        int N = 0;
                        for (int i = 1; i < abs(this->seat.col); i++){
                            if (this->seat.col > 0){
                                if (winup[this->row][i] == 1){
                                    M = i;
                                    N++;
                                }
                            } else {
                                if (windown[this->row][i] == 1){
                                    M = i;
                                    N++;
                                }
                            }
                        }

                        this->tick = B+(M+N+1)*X;
                        this->state = 1;
                        *hallstate(this->row) = 1;
                    }
                }
        } else if (this->state == 1){
            this->tick--;

            if (this->tick == 0){
                *hall(this->row) = *hall(this->row) - 1;

                int M = 0;
                for (int i = 1; i < abs(this->seat.col); i++){
                    if (this->seat.col > 0){
                        if (winup[this->row][i] == 1){
                            M = i;
                        }
                    } else {
                        if (windown[this->row][i] == 1){
                            M = i;
                        }
                    }
                }

                if (this->seat.col > 0){
                    this->col += M+1;
                } else {
                    this->col -= M+1;
                }
                this->tick = X;
                this->state = 2;
                *hallstate(this->row) = 0;

                if (this->seat.col == this->col && this->seat.row == this->row){
                    if (this->col > 0){
                        winup[this->row][this->col] = 1;
                    } else {
                        windown[this->row][-this->col] = 1;
                    }
                    this->state = 3;
                }
            }
        } else if (this->state == 2){
            this->tick--;

            if (this->tick == 0){
                if (this->seat.col > 0){
                    this->col++;
                }
                else {
                    this->col--;
                }

                if (this->seat.col == this->col && this->seat.row == this->row){
                    if (this->col > 0){
                        winup[this->row][this->col] = 1;
                    } else {
                        windown[this->row][-this->col] = 1;
                    }
                    this->state = 3;
                }
                else
                {
                    this->tick = X;
                }
            }
        }


    }
};

Passenger passenger[10001];

bool compare(Passenger p1, Passenger p2){
    return p1.row + p1.col > p2.row + p2.col;
}

void sortP(){
    sort(passenger, passenger+p, compare);
}

void init(int L, int UM, int DM, int P, int *order){
    l = L;
    p = P;

    for (int i = 0; i < l; i++){
        up[i] = UM;
        down[i] = DM;
        *hall(i) = 0;
        *hallstate(i) = 0;
        for(int j=1;j<=UM;j++)
            winup[i][j] = 0;
        for(int j=1;j<=DM;j++)
            windown[i][j] = 0;
    }

    for (int i = 0; i < p; i++){
        passenger[i].setinfo(order[i], i);
        *hall(-i-1) = 1;
        *hallstate(-i-1) = 0;
    }
}

int attempt(){
    int t = 0;

    while (1){
        sortP();

        for (int i = 0; i < p; i++){
            passenger[i].op();
        }

        t++;

        bool sit = true;
        for (int i = 0; i < p; i++){
            if (passenger[i].state != 3){
                sit = false;
            }
        }

        if (sit) break;
    }

    return t;
}

int main() {
    FASTIO;

    int n, a, b, c;
    cin >> n >> a >> b >> c;

    std::ofstream out(to_string(a) + to_string(b) + to_string(c) +"_.csv");

    if (out.is_open()) {
        for (int x = 1; x <= n; x++) {
            for (int ii = 0; ii < 20; ii++){
                int *aa = Permutation(x*2);
                int *bb = Permutation(x*2);
                int *cc = Permutation(x*2);

                int p[1000] = {0};

                for (int i = 0; i < x*6; i++){
                    if (i < x*2){
                        p[i] = aa[i] + (a-1)*x*2;
                    } else if (x*2 <= i && i < x*4){
                        p[i] = bb[i-x*2] + (b-1)*x*2;
                    } else {
                        p[i] = cc[i-x*4] + (c-1)*x*2;
                    }
                }

                init(x, 3, 3, x*6, p);
                out << to_string(x) << ',' << to_string(attempt()) << endl;
            }       
        }
                                    
        // printf("\n");
    }

    out.close();

    return 0;
}