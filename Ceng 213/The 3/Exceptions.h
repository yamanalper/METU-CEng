#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <sstream>

//==========================//
// GRAPH RELATED EXCEPTIONS //
//==========================//
struct DuplicateVertexException
{
    private:
    std::string     vertexName;

    public:
                    DuplicateVertexException(const std::string& vertexName);
    std::string     ToString();
};

struct VertexNotFoundException
{
    private:
    std::string     vertexName;

    public:
                    VertexNotFoundException(const std::string& vertexName);
    std::string     ToString();
};

struct EdgeNotFoundException
{
    private:
    std::string     vertexName;
    std::string     edgeName;
    public:
                    EdgeNotFoundException(const std::string& vertexName,
                                          const std::string& edgeName);
    std::string     ToString();
};

struct SameNamedEdgeException
{
    private:
    std::string     edgeName;
    std::string     vertexFrom;
    std::string     vertexTo;

    public:
                    SameNamedEdgeException(const std::string& edgeName,
                                           const std::string& vertexFrom,
                                           const std::string& vertexTo);
    std::string     ToString();
};

inline DuplicateVertexException::DuplicateVertexException(const std::string& vn)
    : vertexName(vn)
{}

inline std::string DuplicateVertexException::ToString()
{
    std::stringstream ss;
    ss << "Vertex \"" << vertexName << "\" is already in graph.";
    return ss.str();
}

inline VertexNotFoundException::VertexNotFoundException(const std::string& vn)
    : vertexName(vn)
{}

inline std::string VertexNotFoundException::ToString()
{
    std::stringstream ss;
    ss << "Vertex \"" << vertexName << "\" is not found.";
    return ss.str();
}

inline EdgeNotFoundException::EdgeNotFoundException(const std::string& vn,
                                                    const std::string& en)
    : vertexName(vn)
    , edgeName(en)
{}

inline std::string EdgeNotFoundException::ToString()
{
    std::stringstream ss;
    ss << "Edge \"" << edgeName << "\" is not found on Vertex \"" << vertexName << "\".";
    return ss.str();
}

inline SameNamedEdgeException::SameNamedEdgeException(const std::string& en,
                                                      const std::string& vFrom,
                                                      const std::string& vTo)
    : edgeName(en)
    , vertexFrom(vFrom)
    , vertexTo(vTo)
{}

inline std::string SameNamedEdgeException::ToString()
{
    std::stringstream ss;
    ss << "There can not be same named edge \"";
    ss << edgeName << "\" between vertices \"";
    ss <<  vertexFrom  << "\" and \"" << vertexTo << "\".";
    return ss.str();
}

//==========================//
// TABLE RELATED EXCEPTIONS //
//==========================//
struct InvalidTableArgException
{
    std::string     ToString();
};

struct TableCapFullException
{
    private:
    int             elementSize;

    public:
                    TableCapFullException(int elementSize);
    std::string     ToString();
};

inline std::string InvalidTableArgException::ToString()
{
    return std::string("Invalid HashTable argument \"intArray\". "
                       "It should have at least size of 1.");
}

inline TableCapFullException::TableCapFullException(int elementSize)
    : elementSize(elementSize)
{}

inline std::string TableCapFullException::ToString()
{
    std::stringstream ss;
    ss << "HashTable capacity full (" << elementSize << ").";
    return ss.str();
}

#endif // EXCEPTIONS_H