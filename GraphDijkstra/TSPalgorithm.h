#ifndef TSPALGORITHM_H
#define TSPALGORITHM_H
#pragma once
#include "graph.h"

void RestoreWay(QVector<QVector<int>>& adj, QVector<int>& path, int size, QVector<QVector<float>>& masked) {
    size_t i = 0;
    int mask = (1 << size) - 1;
    path.push_back(0);
    while (mask != 0) {
        for (size_t j = 0; j < size; j++) {
            if (adj[i][j]) {
                if ((mask & (1 << j))) {
                    if (masked[mask][i] == adj[i][j] + masked[mask ^ (1 << j)][j]) {
                        path.push_back(j);
                        i = j;
                        mask = mask ^ (1 << j);
                    }
                }
            }
        }
    }
}

float MaskTSP(int i, int mask, QVector<QVector<int>>& adj, int size, QVector<QVector<float>>& masked) {
    if (masked[mask][i] != Q_INFINITY) return masked[mask][i];

    for (size_t j = 0; j < size; j++) {
        if (adj[i][j]) {
            if ((mask & (1 << j))) {
                masked[mask][i] = std::min(masked[mask][i], MaskTSP(j, mask - (1 << j), adj, size, masked) + adj[i][j]);

            }
        }
    }
    return masked[mask][i];
}

QString TSP(Graph graph) {
    QString res = "Кратчайший возможный путь: ";
    int size = graph.getVertexList().size();

    QVector<QVector<float>>masked((1 << size), QVector<float>(size));

    QVector<QVector<int>> adj = graph.getAdj();

    for (size_t i = 0; i < size; i++)
        for (size_t mask = 0; mask < (1 << size); mask++)
            masked[mask][i] = Q_INFINITY;

    masked[0][0] = 0;

    int ans = MaskTSP(0, (1 << size) - 1, adj, size, masked);
    res += QString::number( ans );
    res += "\nПосещаемые пункты: ";

    QVector<int> path;
    RestoreWay(adj, path, size, masked);
    for (size_t i = 0; i < size; ++i) res  += QString::number(path[i]) + " -> ";
    res += "0";
    return res;
}
#endif // TSPALGORITHM_H
