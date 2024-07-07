/* -*-c++-*- */
/* osgEarth - Geospatial SDK for OpenSceneGraph
* Copyright 2018 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgEarth/catch.hpp>

#include <osgEarth/ImageLayer>
#include <osgEarth/Registry>
#include <osgEarth/GDAL>

using namespace osgEarth;

TEST_CASE( "ImageLayers can be created" )
{
    GDALImageLayer* layer = new GDALImageLayer();
    layer->setName("World");
    layer->setURL("../data/world.tif");

    Status status = layer->open();
    REQUIRE( status.isOK() );

    SECTION("Profiles are correct")
    {
        const Profile* profile = layer->getProfile();
        REQUIRE(profile != nullptr);
        REQUIRE(profile->isEquivalentTo(Profile::create(Profile::GLOBAL_GEODETIC)));
    }

    SECTION("Images are read correctly")
    {
        TileKey key(0, 0, 0, layer->getProfile());
        GeoImage image = layer->createImage(key);
        REQUIRE(image.valid());
        REQUIRE(image.getImage()->s() == 256);
        REQUIRE(image.getImage()->t() == 256);
        REQUIRE(image.getExtent() == key.getExtent());
    }
}

TEST_CASE("Attribution works")
{
    std::string attribution = "Attribution test";

    GDALImageLayer* layer = new GDALImageLayer();
    layer->setURL("../data/world.tif");
    layer->setAttribution(attribution);

    Status status = layer->open();

    REQUIRE(status.isOK());
    REQUIRE(layer->getAttribution() == attribution);
}