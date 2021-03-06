/**
 * @file CADToPointCloud.h
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

#pragma once
#ifndef _CAD_TO_POINTCLOUD_H
#define _CAD_TO_POINTCLOUD_H

#include <Utils.h>

#include <pcl/io/vtk_lib_io.h>


/**
 * @brief This class is used to obtain pcl::PointCloud from CAD file.
 *        Supported formats: .OBJ
 *
 */
class CADToPointCloud
{
    typedef pcl::PointCloud<pcl::PointXYZ> PointCloudXYZ;
    typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloudRGB;

public:
    /**
     * @brief Construct a new CADToPointCloud object
     *
     * @param[in] cad_file_path Supported formats: .OBJ .PLY
     */
    CADToPointCloud(const std::string& cad_file_path, int sample_points);

    /**
     * @brief Destroy the CADToPointCloud object
     *
     */
    ~CADToPointCloud(){};

    /**
     * @brief convert cloud defined by path in construstor into cloud
     * 
     * @param[out] cloud to store cad converted to cloud
     */
    void convertCloud(PointCloudRGB::Ptr cloud);

private:

    /** @brief cad file path extension to check formats. */
    std::string extension_;

    /** @brief path to CAD file to be converted. */
    std::string cad_file_path_;

    /** @brief number of points to sample CAD mesh. */
    int SAMPLE_POINTS_;
    
    // Following methods are extracted from Point Cloud Library (PCL)
    //                --> pcl/tools/mesh_sampling.cpp
    //
    //  Software License Agreement (BSD License)
    //
    //  Point Cloud Library (PCL) - www.pointclouds.org
    //  Copyright (c) 2010-2011, Willow Garage, Inc.
    //  All rights reserved.
    //
    //  Redistribution and use in source and binary forms, with or without
    //  modification, are permitted provided that the following conditions
    //  are met:
    //
    //   * Redistributions of source code must retain the above copyright
    //     notice, this list of conditions and the following disclaimer.
    //   * Redistributions in binary form must reproduce the above
    //     copyright notice, this list of conditions and the following
    //     disclaimer in the documentation and/or other materials provided
    //     with the distribution.
    //   * Neither the name of the copyright holder(s) nor the names of its
    //     contributors may be used to endorse or promote products derived
    //     from this software without specific prior written permission.
    //
    //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    //  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    //  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    //  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    //  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    //  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    //  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    //  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    //  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    //  POSSIBILITY OF SUCH DAMAGE.
    //

    /** @brief Extracted from Point Cloud Library (PCL)  --> pcl/tools/mesh_sampling.cpp
     *         \n Copyright (c) 2010-2011, Willow Garage, Inc. */
    void uniformSampling(vtkSmartPointer<vtkPolyData> polydata, size_t n_samples, PointCloudRGB& cloud_out);

    /** @brief Extracted from Point Cloud Library (PCL)  --> pcl/tools/mesh_sampling.cpp
     *         \n Copyright (c) 2010-2011, Willow Garage, Inc. */
    void randPSurface(vtkPolyData* polydata, std::vector<double>* cumulativeAreas, double totalArea, Eigen::Vector4f& p);

    /** @brief Extracted from Point Cloud Library (PCL)  --> pcl/tools/mesh_sampling.cpp
     *         \n Copyright (c) 2010-2011, Willow Garage, Inc. */
    void randomPointTriangle(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3,
                            Eigen::Vector4f& p);

    /** @brief Extracted from Point Cloud Library (PCL)  --> pcl/tools/mesh_sampling.cpp
     *         \n Copyright (c) 2010-2011, Willow Garage, Inc. */
    double uniformDeviate(int seed);
};

#endif
