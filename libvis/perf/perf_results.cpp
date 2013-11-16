/**
 * @file perf_results.cpp
 * @brief Produce performance tests results.
 * @author Paolo D'Apice
 */

#include "dir.h"
#include "perf_data.hpp"
#include "perf_serialization.hpp"

#include <boost/filesystem/path.hpp>
#ifdef HAVE_GNUPLOT_IOSTREAM
#  include <gnuplot-iostream.h>
#endif

namespace fs = boost::filesystem;

/// Directory containing XML results.
static const fs::path XML_DIR = fs::path(DATA_DIR) / "perf";

typedef std::pair<std::string, std::string> FileVariablePair;

static const std::array<FileVariablePair, 6> PAIRS = {
        std::make_pair("build_index.xml", "numData"),
        std::make_pair("build_index_numtrees.xml", "numTrees"),
        std::make_pair("query_index.xml", "numData"),
        std::make_pair("query_index_numtrees.xml", "numTrees"),
        std::make_pair("query_index_neighbors.xml", "numNeigbors"), // TODO fix typo
        std::make_pair("query_index_ann.xml", "maxComparisons"),
        //std::make_pair("descriptors.xml", "descriptor") // TODO graph for descriptors
};

#define GNUPLOT_OUTPUT_FILE

#ifdef HAVE_GNUPLOT_IOSTREAM

/// Read Gnuplot script from file.
std::string readPlotFile(const std::string& name) {
    std::ifstream ifs(name.c_str());
    return std::string( (std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()) );
}

/// Build graph and save to file.
void makeGraph(const perf::ResultsTable& table,
        const std::string& script,
        const std::string& output) {
    Gnuplot plot;
#ifdef GNUPLOT_OUTPUT_FILE
    plot << "set term post eps color\nset output '" << output << "'\n";
#endif
    plot << script;
    plot.send1d(table.records);
    plot.send1d(table.records);

    std::cout << "Plot saved to file: " << output << std::endl;
}
#endif

/// Process XML results and make a table-like object.
void process(const std::string& input, const std::string& name) {
    fs::path file = XML_DIR / input;

    perf::TimingsVector timings;
    perf::load(file.string(), timings);

    perf::ResultsTable table(name, timings);
    perf::save("results_" + input, table);

#ifdef HAVE_GNUPLOT_IOSTREAM
    fs::path basename = file.filename();
    std::string savefile = "graph_" + basename.replace_extension("eps").string();
    fs::path plotfile = fs::path(PLOT_DIR) / basename.replace_extension("gp");
    std::string plot = readPlotFile(plotfile.string());
    makeGraph(table, plot, savefile);
#endif
}

int main(int, char**) {
    for (const FileVariablePair& pair : PAIRS) {
        process(pair.first, pair.second);
    }
}

