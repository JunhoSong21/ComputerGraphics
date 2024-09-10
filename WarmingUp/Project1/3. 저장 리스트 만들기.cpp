#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

const int MAX_POINTS = 20;

struct Point {
    int x, y, z;
};

Point points[MAX_POINTS];
int pointCount = 0;
bool sortedAsc = true;

void printList();
void addPointToFront(int x, int y, int z);
void addPointToBack(int x, int y, int z);
void deletePointFromFront();
void deletePointFromBack();
void printPointCount();
void clearList();
void printFarthestPoint();
void printNearestPoint();
void sortAscending();
void sortDescending();
double distanceFromOrigin(const Point& p);

int main() {
    char Option;
    int x, y, z;

    while (true) {
        printList();
        std::cout << "명령어를 입력하세요 : ";
        std::cin >> Option;

        switch (Option) {
        case '+':
        {
            std::cin >> x >> y >> z;
            addPointToFront(x, y, z);
            break;
        }
        case '-': {
            deletePointFromFront();
            break;
        }
        case 'e': {
            std::cin >> x >> y >> z;
            addPointToBack(x, y, z);
            break;
        }
        case 'd': {
            deletePointFromBack();
            break;
        }
        case 'l': {
            printPointCount();
            break;
        }
        case 'c': {
            clearList();
            break;
        }
        case 'm': {
            printFarthestPoint();
            break;
        }
        case 'n': {
            printNearestPoint();
            break;
        }
        case 'a': {
            sortAscending();
            break;
        }
        case 's': {
            sortDescending();
            break;
        }
        case 'q':
        {
            std::cout << "프로그램을 종료합니다." << std::endl;
            return 0;
        }
        default:
            std::cout << "잘못된 입력입니다." << std::endl;
            break;
        }
    }

    return 0;
}

void printList() {
    std::cout << "인덱스| x  y  z\n";
    std::cout << "----------------\n";
    for (int i = MAX_POINTS - 1; i >= 0; --i) {
        if (i < pointCount) {
            std::cout << std::setw(5) << i << " | "
                << std::setw(2) << points[i].x << " "
                << std::setw(2) << points[i].y << " "
                << std::setw(2) << points[i].z << "\n";
        }
        else {
            std::cout << std::setw(5) << i << " |  -  -  -\n";
        }
    }
}

void addPointToFront(int x, int y, int z) {
    if (pointCount < MAX_POINTS) {
        for (int i = pointCount; i > 0; --i) {
            points[i] = points[i - 1];
        }
        points[0] = { x, y, z };
        ++pointCount;
    }
    else {
        std::cout << "리스트가 가득 찼습니다." << std::endl;
    }
}

void addPointToBack(int x, int y, int z) {
    if (pointCount < MAX_POINTS) {
        points[pointCount++] = { x, y, z };
    }
    else {
        std::cout << "리스트가 가득 찼습니다." << std::endl;
    }
}

void deletePointFromFront() {
    if (pointCount > 0) {
        for (int i = 0; i < pointCount - 1; ++i) {
            points[i] = points[i + 1];
        }
        --pointCount;
    }
    else {
        std::cout << "리스트가 비어 있습니다." << std::endl;
    }
}

void deletePointFromBack() {
    if (pointCount > 0) {
        --pointCount;
    }
    else {
        std::cout << "리스트가 비어 있습니다." << std::endl;
    }
}

void printPointCount() {
    std::cout << "현재 저장된 점의 개수 : " << pointCount << std::endl;
}

void clearList() {
    pointCount = 0;
}

void printFarthestPoint() {
    if (pointCount > 0) {
        int farthestIndex = 0;
        double maxDist = distanceFromOrigin(points[0]);

        for (int i = 1; i < pointCount; ++i) {
            double dist = distanceFromOrigin(points[i]);
            if (dist > maxDist) {
                maxDist = dist;
                farthestIndex = i;
            }
        }
        std::cout << "가장 먼 점: (" << points[farthestIndex].x << ", "
            << points[farthestIndex].y << ", "
            << points[farthestIndex].z << ")\n";
    }
    else {
        std::cout << "리스트가 비어 있습니다." << std::endl;
    }
}

void printNearestPoint() {
    if (pointCount > 0) {
        int nearestIndex = 0;
        double minDist = distanceFromOrigin(points[0]);

        for (int i = 1; i < pointCount; ++i) {
            double dist = distanceFromOrigin(points[i]);
            if (dist < minDist) {
                minDist = dist;
                nearestIndex = i;
            }
        }
        std::cout << "가장 가까운 점: (" << points[nearestIndex].x << ", "
            << points[nearestIndex].y << ", "
            << points[nearestIndex].z << ")\n";
    }
    else {
        std::cout << "리스트가 비어 있습니다." << std::endl;
    }
}

void sortAscending() {
    std::sort(points, points + pointCount, [](const Point& a, const Point& b) {
        return distanceFromOrigin(a) < distanceFromOrigin(b);
        });
    sortedAsc = true;
}

void sortDescending() {
    std::sort(points, points + pointCount, [](const Point& a, const Point& b) {
        return distanceFromOrigin(a) > distanceFromOrigin(b);
        });
    sortedAsc = false;
}

double distanceFromOrigin(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}
