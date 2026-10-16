#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Cone {
    string id;
    double x;
    double y;
};

struct MergedCone {
    string new_id;
    double x;
    double y;
    vector<string> old_ids; // ID pachołków połączonych
};

struct Point {
    double x, y;
    string id;
};

// orientacja skrętu
double crossProduct(const Point& o, const Point& a, const Point& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

double distance(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void findConvexHull(const vector<MergedCone>& result_cones) {
    if (result_cones.size() < 3) {
        return;
    }

    int m = result_cones.size();
    vector<Point> points(m);
    for (int i = 0; i < m; ++i) {
        points[i] = {result_cones[i].x, result_cones[i].y, result_cones[i].new_id};
    }
    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    vector<Point> hull(2 * m);
    int k = 0;

    // dolna otoczka
    for (int i = 0; i < m; ++i) {
        while (k >= 2 && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    // górna otoczka
    for (int i = m - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    hull.resize(k - 1); // usunięcie punktu pocz na końcu

    // obwód
    double perimeter = 0.0;
    for (size_t i = 0; i < hull.size(); ++i) {
        Point p1 = hull[i];
        Point p2 = hull[(i + 1) % hull.size()];
        perimeter += distance(p1, p2);
    }

    cout << "Dlugosc otoczki: " << fixed << setprecision(3) << perimeter << endl;
    cout << "Pacholki tworzace otoczke: ";
    for (const auto& p : hull) {
        cout << p.id << " ";
    }
    cout << endl;
}

vector<MergedCone> deduplicateCones(const vector<Cone>& cones, double d_min) {
    int n = cones.size();
    vector<bool> visited(n, false); 
    vector<MergedCone> result_cones; 
    int new_id_counter = 1;

    for (int i = 0; i < n; ++i) {
        if (visited[i]) continue;
        
        vector<int> group_indices;
        group_indices.push_back(i);
        visited[i] = true;
        
        bool added_new_to_group = true;
        while (added_new_to_group) {
            added_new_to_group = false;

            for (int j = 0; j < n; ++j) {
                if (visited[j]) continue;

                for (int group_idx : group_indices) {
                    double dx = cones[group_idx].x - cones[j].x;
                    double dy = cones[group_idx].y - cones[j].y;
                    double dist = sqrt(dx * dx + dy * dy);

                    if (dist <= d_min) { 
                        visited[j] = true;
                        group_indices.push_back(j);
                        added_new_to_group = true; 
                        break; 
                    }
                }
            }
        }
        
        double sum_x = 0;
        double sum_y = 0;
        vector<string> merged_ids;

        for (int idx : group_indices) {
            sum_x += cones[idx].x;
            sum_y += cones[idx].y;
            merged_ids.push_back(cones[idx].id);
        }

        MergedCone mc;
        mc.new_id = "N" + to_string(new_id_counter++);
        mc.x = sum_x / group_indices.size();
        mc.y = sum_y / group_indices.size();
        mc.old_ids = merged_ids;

        result_cones.push_back(mc);
    }

    cout << "Liczba nowych pacholkow: " << result_cones.size() << "\n\n";

    for (const auto& mc : result_cones) {
        cout << mc.new_id 
            << ", (" << mc.x << ", " << mc.y << "), " 
            << "utworzony z: ";
        
        for (const auto& old_id : mc.old_ids) {
            cout << old_id << " ";
        }
        cout << endl;
    }
    
    return result_cones;
}

int main() {
    ifstream file("example_input.txt");
    if (!file.is_open()) {
        return 1;
    }

    int n;
    double d_min;
    file >> n >> d_min;

    vector<Cone> cones(n);
    for (int i = 0; i < n; ++i) {
        file >> cones[i].id >> cones[i].x >> cones[i].y;
    }
    file.close();

    // deduplikacja
    vector<MergedCone> result_cones = deduplicateCones(cones, d_min);
    
    // otoczka
    findConvexHull(result_cones);

    return 0;
}