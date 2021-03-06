package Otsu_Thresholder.Control;

import ij.plugin.DICOM;

import java.awt.*;
import java.awt.color.ColorSpace;
import java.awt.image.*;
import java.io.IOException;
import java.util.Hashtable;

/**
 * Created by kareem on 7/13/17.
 */
public class Thresholder {
    private double threshold;
    private double background;
    private double foreground;
    private double PixelsValueSum;
    private double weightSum;
    private double meanSum;
    private double varianceSum;
    private double Bweight;
    private double Bmean;
    private double Bvariance;
    private double Fweight;
    private double Fmean;
    private double Fvariance;
    private double bcvariance;
    private double MaxBcvariance;
    private Histogram histogram;
    private long[] HistogramData;

    public Thresholder() {
        this.MaxBcvariance = 0;
        this.PixelsValueSum = 0;

    }

//TODO #Amr
    //          ONLY and ONLY implement one of them
    //          iam suggesting the first one because it more native

    /**
     * use Otsu thresholding algo to segment image
     *
     * @param input a gray scale image to start segmentation
     * @return new instance of buffered image
     */
    public BufferedImage startThresholding(BufferedImage input) throws IOException {
        this.histogram = getHistogram(input);
        this.HistogramData = histogram.getGrayHistogram();


        for (int i = 0; i < 256; i++)
            this.PixelsValueSum += this.HistogramData[i];

        for (int i = 0; i < 256; i++) {

// Reset Weight Values
            this.Bweight = 0;
            this.Fweight = 0;
            this.weightSum = 0;
            this.meanSum = 0;
            this.varianceSum = 0;

            // For Background

            for (int Bi = 0; Bi < i; Bi++) {
                this.weightSum += this.HistogramData[Bi];
                this.meanSum += Bi * this.HistogramData[Bi];
            }
            this.Bweight = this.weightSum / this.PixelsValueSum;
            this.Bmean = this.meanSum / this.weightSum;
            this.meanSum = 0;

            // For Foreground
            this.Fweight = 1 - Bweight;
            for (int Fi = i; Fi < 256; Fi++) {
                this.meanSum += Fi * this.HistogramData[Fi];
            }
            this.Fmean = this.meanSum / (this.PixelsValueSum - this.weightSum);

            this.bcvariance = this.Bweight * this.Fweight * (this.Bmean - this.Fmean) * (this.Bmean - this.Fmean);

            if (this.bcvariance > this.MaxBcvariance) {
                this.MaxBcvariance = this.bcvariance;
                this.threshold = i;
            }

        }
        Raster raster = input.getData();
        DataBuffer buffer = raster.getDataBuffer();
        DataBufferByte byteBuffer = (DataBufferByte) buffer;
        byte[] srcData = byteBuffer.getData(0);
        byte[] thresholdingImage = new byte[srcData.length];
        for (int j = 0; j < srcData.length; j++)
            thresholdingImage[j] = ((srcData[j] & 0xFF) >= this.threshold) ? (byte) 255 : 0;

        DataBufferByte var5 = new DataBufferByte(thresholdingImage, thresholdingImage.length, 0);
        PixelInterleavedSampleModel var6 = new PixelInterleavedSampleModel(0, input.getWidth(), input.getHeight(), 1, input.getWidth(), new int[]{0});
        ColorSpace var7 = ColorSpace.getInstance(1003);
        ComponentColorModel var8 = new ComponentColorModel(var7, new int[]{8}, false, false, 1, 0);
        WritableRaster var9 = Raster.createWritableRaster(var6, var5, (Point)null);

        return   new BufferedImage(var8, var9, false, (Hashtable)null);
    }


    public DICOM startThresholding(DICOM input) {
        return input;
    }

    //TODO #hazem
    //              ask Amr for what u shall do
    public Histogram getHistogram(BufferedImage input) {
        Histogram histogram = new Histogram();

        histogram.GenerateHistogram(input);
        return histogram;
    }

}
