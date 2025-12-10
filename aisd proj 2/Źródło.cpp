#include <iostream>
#include <cstdlib>
#include <climits>

struct Node {
    long long cost;
    int x, y;
};

struct Wyciag {
    int starX, starY;
    int finaX, finaY;
    int czas;
    int czasOdjazd;
};

struct MinHeap {
    Node* data;
    int maxSize;
    int size;

    MinHeap(int cap) {
        maxSize = cap;
        size = 0;
        data = (Node*)std::malloc(sizeof(Node) * (maxSize + 1));
    }

    ~MinHeap() {
        std::free(data);
    }

    void swap(int i, int j) {
        Node temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    void push(const Node& node) {
        if (size >= maxSize) {
            return;
        }
        size++;
        data[size] = node;
        upHeap(size);
    }

    void pop() {
        if (size == 0) {
            return;
        }
        data[1] = data[size];
        size--;
        downHeap(1);
    }

    bool empty() const {
        return size == 0;
    }

    Node top() const {
        return data[1];
    }

    void upHeap(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (data[parent].cost > data[index].cost) {
                swap(parent, index);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void downHeap(int index) {
        while (true) {
            int left = 2 * index;
            int right = 2 * index + 1;
            int smallest = index;
            if (left <= size && data[left].cost < data[smallest].cost) {
                smallest = left;
            }
            if (right <= size && data[right].cost < data[smallest].cost) {
                smallest = right;
            }
            if (smallest != index) {
                swap(index, smallest);
                index = smallest;
            }
            else {
                break;
            }
        }
    }
};

long long** createDist(int width, int height) {
    long long** arr = new long long* [width];
    for (int i = 0; i < width; i++) {
        arr[i] = new long long[height];
        for (int j = 0; j < height; j++) {
            arr[i][j] = LLONG_MAX;
        }
    }
    return arr;
}

int main() {
    int width, height;
    int startX, startY, finalX, finalY;
    int wyciagi;

    if (scanf_s("%d %d", &width, &height) != 2) return 1;
    if (scanf_s("%d %d", &startX, &startY) != 2) return 1;
    if (scanf_s("%d %d", &finalX, &finalY) != 2) return 1;

    if (startX < 0 || startX >= width || startY < 0 || startY >= height) {
        return 1;
    }
    if (finalX < 0 || finalX >= width || finalY < 0 || finalY >= height) {
        return 1;
    }

    if (scanf_s("%d", &wyciagi) != 1) return 1;
    Wyciag* w = new Wyciag[wyciagi];

    for (int i = 0; i < wyciagi; ++i) {
        scanf_s("%d %d %d %d %d %d", &w[i].starX, &w[i].starY,
            &w[i].finaX, &w[i].finaY,
            &w[i].czas, &w[i].czasOdjazd);
    }

    int** H = new int* [height];
    for (int i = 0; i < height; ++i) {
        H[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            scanf_s("%d", &H[i][j]);
        }
    }

    int totalCells = width * height;
    int* head = new int[totalCells];
    int* nxt = new int[wyciagi];
    for (int i = 0; i < totalCells; ++i) head[i] = -1;
    for (int i = 0; i < wyciagi; ++i) {
        int idx = w[i].starY * height + w[i].starX;
        nxt[i] = head[idx];
        head[idx] = i;
    }

    // Dijkstra
    long long** dist = createDist(width, height);
    MinHeap heap(width * height + wyciagi + 5);

    dist[startX][startY] = 0;
    heap.push({ 0, startX, startY });

    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0, 0, 1, -1 };

    while (!heap.empty()) {
        Node u = heap.top();
        heap.pop();
        int x = u.x, y = u.y;
        long long d = u.cost;
        if (d > dist[x][y]) continue;
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
            int A = H[ny][nx];
            int B = H[y][x];
            long long step = (A > B ? (A - B + 1) : 1);
            if (dist[nx][ny] > d + step) {
                dist[nx][ny] = d + step;
                heap.push({ dist[nx][ny], nx, ny });
            }
        }
        // wyciagi
        int cellIdx = x * height + y;
        for (int i = head[cellIdx]; i != -1; i = nxt[i]) {
            Wyciag& lift = w[i];
            long long wait = (lift.czasOdjazd - (d % lift.czasOdjazd)) % lift.czasOdjazd;
            long long nd = d + wait + lift.czas;
            int fx = lift.finaX, fy = lift.finaY;
            if (dist[fx][fy] > nd) {
                dist[fx][fy] = nd;
                heap.push({ nd, fx, fy });
            }
        }
    }

    if (dist[finalX][finalY] == LLONG_MAX) {
        std::printf("Brak dostepnej trasy\n");
    }
    else {
        std::printf("Minimalny czas: %lld minut\n", dist[finalX][finalY]);
    }

    for (int i = 0; i < width; ++i) {
        delete[] H[i];
        delete[] dist[i];
    }
    delete[] H;
    delete[] dist;
    delete[] w;
    delete[] head;
    delete[] nxt;

    return 0;
}
