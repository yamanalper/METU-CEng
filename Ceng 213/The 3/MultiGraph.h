#ifndef MULTI_GRAPH_H
#define MULTI_GRAPH_H

#include <vector>
#include <string>

struct GraphEdge
{
    std::string name;       // Name of the vertex
    float       weight[2];  // Weights of the edge
                            // (used on shortest path)
    int         endVertexIndex;
};

struct GraphVertex
{
    std::vector<GraphEdge> edges; // Adjacency List
    std::string            name;  // Name of the vertex
};

class MultiGraph
{
    private:
    std::vector<GraphVertex>    vertexList;

    static float Lerp(float w0, float w1, float alpha);

    protected:
    public:
    // Constructors & Destructor
                MultiGraph();
                MultiGraph(const std::string& filePath);

    // Connect Vertices
    void        InsertVertex(const std::string& vertexName);
    void        RemoveVertex(const std::string& vertexName);

    // Connect Vertices
    void        AddEdge(const std::string& edgeName,
                        const std::string& vertexFromName,
                        const std::string& vertexToName,
                        float weight0, float weight1);
    void        RemoveEdge(const std::string& edgeName,
                           const std::string& vertexFromName,
                           const std::string& vertexToName);

    // Shortest Path Functions
    bool        HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight) const;
    bool        FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                     const std::string& vertexNameFrom,
                                     const std::string& vertexNameTo,
                                     float heuristicWeight,
                                     const std::vector<std::string>& edgeNames) const;

    // Other functions
    int         BiDirectionalEdgeCount() const;
    int         MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const;

    // Implemented Functions for Debugging
    void        PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                          float heuristicWeight,
                          bool sameLine = false) const;
    void        PrintEntireGraph() const;
    
    bool        findEdgeWeight(const std::string& edgeName,
                                const std::string& vertexFromName,
                                const std::string& vertexToName,
                                std::vector<float>& weights);
                                
    int         findIndex(const std::string& vertexFromName) const;
};

#endif // MULTI_GRAPH_H