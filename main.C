#include <PNMreader.h>
#include <PNMwriter.h>
#include <filter.h>
#include <logging.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using std::cerr;
using std::endl;
using std::ofstream;

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        exit(EXIT_FAILURE);
    }

    PNMreader reader(argv[1]);
    Shrinker sr;
    Shrinker sr2;
    Shrinker sr3;
    Blur br;
    Blur br2;
    Blender bl;
    Blender bl2;
    Mirror mr;
    Mirror mr2;
    Mirror mr3;
    Rotate rt;
    Rotate rt2;
    Subtract st;
    Subtract st2;
    Grayscale gr;
    TBCombine tb;
    TBCombine tb1;
    TBCombine tb2;
    TBCombine tb3;
    TBCombine tb4;
    TBCombine tb5;
    TBCombine tb6;
    TBCombine tb7;
    TBCombine tb8;
    TBCombine tb9;
    TBCombine tb10;
    TBCombine tb11;
    TBCombine tb12;
    LRCombine lr;
    LRCombine lr2;
    LRCombine lr3;
    LRCombine lr4;
    LRCombine lr5;
    Color gold(1786, 1344, 255, 215, 0);
    Color random(1786, 1344, 121, 22, 150);
    bl.SetFactor(0.6);
    bl2.SetFactor(0.5);
    tb.SetInput(reader.GetOutput());
    tb.SetInput2(reader.GetOutput());
    tb1.SetInput(tb.GetOutput());
    tb1.SetInput2(tb.GetOutput());
    tb5.SetInput(tb1.GetOutput());
    tb5.SetInput2(tb1.GetOutput());
    
    tb6.SetInput(random.GetOutput());
    tb6.SetInput2(random.GetOutput());
    tb7.SetInput(tb.GetOutput());
    tb7.SetInput2(tb6.GetOutput());
    tb8.SetInput(tb7.GetOutput());
    tb8.SetInput2(tb6.GetOutput());
    tb9.SetInput(tb8.GetOutput());
    tb9.SetInput2(tb.GetOutput());
    
    tb10.SetInput(tb.GetOutput());
    tb10.SetInput2(reader.GetOutput());
    tb11.SetInput(tb10.GetOutput());
    tb11.SetInput2(tb6.GetOutput());
    tb12.SetInput(tb11.GetOutput());
    tb12.SetInput2(tb10.GetOutput());


    lr.SetInput(tb12.GetOutput());
    lr.SetInput2(tb5.GetOutput());
    lr3.SetInput(tb9.GetOutput());
    lr3.SetInput2(lr.GetOutput());
    lr4.SetInput(tb9.GetOutput());
    lr4.SetInput2(lr3.GetOutput());

    mr.SetInput(lr4.GetOutput());
    lr2.SetInput(mr.GetOutput());
    lr2.SetInput2(lr4.GetOutput());  

    sr.SetInput(lr2.GetOutput());
    sr2.SetInput(sr.GetOutput());
    sr3.SetInput(sr2.GetOutput());

    st.SetInput(gold.GetOutput());
    st.SetInput2(sr3.GetOutput());
    gr.SetInput(reader.GetOutput());
    br.SetInput(gr.GetOutput());
    br2.SetInput(br.GetOutput());
    bl.SetInput(br2.GetOutput());
    bl.SetInput2(st.GetOutput());
    rt.SetInput(bl.GetOutput());
    rt2.SetInput(rt.GetOutput());
    bl2.SetInput(rt2.GetOutput());
    bl2.SetInput2(bl.GetOutput());
    st2.SetInput(rt2.GetOutput());
    st2.SetInput2(bl.GetOutput());

    Image *finalImage = bl2.GetOutput();

    try
    {
        finalImage->Update();
    }
    catch (DataFlowException &)
    {
        ofstream ofile("my_exception");
        if (ofile.fail())
        {
             cerr << "Something is wrong ... can't open my_exception"
                  << " for opening" << endl;
             exit(EXIT_FAILURE);
        }
        ofile << "Exception found!" << endl;
        exit(EXIT_SUCCESS);
    }

    CheckSum cs;
    cs.SetInput(finalImage);
    cs.OutputCheckSum("my_checksum");

    if (argc == 3)
    {
        PNMwriter writer;
        writer.SetInput(finalImage);
        writer.Write(argv[2]);
    }
    Logger::Finalize();
}
