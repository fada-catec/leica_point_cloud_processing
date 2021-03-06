/**
 * @file test_filter.cpp
 * @copyright Copyright (c) 2020, FADA-CATEC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#include <Filter.h>
#include <Utils.h>
#include <gtest/gtest.h>
#include <exception>
#include <pcl/filters/filter.h>

typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloudRGB;

class TestFilter : public ::testing::Test
{ 
protected:

    PointCloudRGB::Ptr cloudRGB {new PointCloudRGB};

    TestFilter()
    {
        cubePointCloud(cloudRGB, 5, 5000, 0);
    }

    void cubePointCloud(PointCloudRGB::Ptr cloud, float dim, int nsamples, int x)
    {
        pcl::PointXYZRGB p_rgb;
        for(int i=0; i<nsamples; i++)
        {
            p_rgb.x = x + dim * (double)std::rand() / (double)RAND_MAX;
            p_rgb.y = x + dim * (double)std::rand() / (double)RAND_MAX;
            p_rgb.z = x + dim * (double)std::rand() / (double)RAND_MAX;
            p_rgb.r = 255;
            p_rgb.g = 255;
            p_rgb.b = 255;
            cloud->push_back(p_rgb);
        } 
    }
    void floorPointCloud(PointCloudRGB::Ptr cloud, float dim)
    {
        pcl::PointXYZRGB p_rgb;
        for (float i=0; i<dim; i+=0.1){
            for (float j=0; j<dim; j+=0.1){
                p_rgb.x = i; 
                p_rgb.y = j; 
                p_rgb.z = 0; 
                p_rgb.r = 255; 
                p_rgb.g = 255; 
                p_rgb.b = 255; 
                cloud->push_back(p_rgb);
            }
        }
    }
};

TEST_F(TestFilter, testDownsample)
{
    double res = Utils::computeCloudResolution(cloudRGB);
    float leaf_size = 2*res;

    PointCloudRGB::Ptr cloudRGBfiltered {new PointCloudRGB};

    Filter cloud_filter(leaf_size);
    cloud_filter.setLeafSize(leaf_size);
    cloud_filter.downsampleCloud(cloudRGB, cloudRGBfiltered);
    
    double end_res = Utils::computeCloudResolution(cloudRGBfiltered);

    EXPECT_GT(end_res, res);
}

TEST_F(TestFilter, testBox)
{
    double res = Utils::computeCloudResolution(cloudRGB);
    float noise_th = res*5;

    PointCloudRGB::Ptr cloudRGBfiltered {new PointCloudRGB};

    Filter cloud_filter;
    cloud_filter.setNoiseThreshold(noise_th);
    cloud_filter.setCloudCenter(Eigen::Vector3f(0,0,0));
    cloud_filter.filterNoise(cloudRGB, cloudRGBfiltered);

    double end_res = Utils::computeCloudResolution(cloudRGBfiltered);

    // EXPECT_GT(end_res, res);
    ASSERT_TRUE(Utils::isValidCloud(cloudRGBfiltered));
}

TEST_F(TestFilter, testDifference)
{
    double res = Utils::computeCloudResolution(cloudRGB);
    PointCloudRGB::Ptr source_cloud = cloudRGB;
    PointCloudRGB::Ptr target_cloud{new PointCloudRGB};         
    cubePointCloud(target_cloud, 5, 5000, 2);
    
    PointCloudRGB::Ptr cloudRGBDiff {new PointCloudRGB};
    Filter::removeFromCloud(target_cloud, source_cloud, res, cloudRGBDiff);
  
    ASSERT_TRUE(Utils::isValidCloud(cloudRGBDiff));
}

TEST_F(TestFilter, testFloor)
{
    PointCloudRGB::Ptr source_cloud{new PointCloudRGB};
    PointCloudRGB::Ptr floor_cloud{new PointCloudRGB};         
    floorPointCloud(floor_cloud, 10);

    *source_cloud = *cloudRGB+*floor_cloud;

    PointCloudRGB::Ptr cloudRGBfiltered{new PointCloudRGB};
    Filter cloud_filter;
    cloud_filter.setFloorThreshold(0.01);
    cloud_filter.filterFloor(source_cloud, cloudRGBfiltered);

    EXPECT_GT(source_cloud->size(), cloudRGBfiltered->size());
    ASSERT_TRUE(Utils::isValidCloud(cloudRGBfiltered));
}

TEST_F(TestFilter, testExtractIndices)
{
    PointCloudNormal::Ptr normals{new PointCloudNormal};
    Utils::getNormals(cloudRGB, 0.01, normals);

    pcl::IndicesPtr indices(new std::vector<int>);
    pcl::removeNaNFromPointCloud(*cloudRGB, *cloudRGB, *indices);
    Filter::extractIndices(cloudRGB, cloudRGB, indices);
    pcl::removeNaNNormalsFromPointCloud(*normals, *normals, *indices);
    Filter::extractIndices(normals, normals, indices);
}

TEST_F(TestFilter, testOutlierRemove)
{
    PointCloudRGB::Ptr cloudRGBfiltered{new PointCloudRGB};

    Filter cloud_filter;
    cloud_filter.outlierRemove(cloudRGB, cloudRGBfiltered);
    EXPECT_GT(cloudRGB->size(), cloudRGBfiltered->size());
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}