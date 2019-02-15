# ofxGCodePlotter
ofxGCodePlotter is an "ofPath to GCode" converter. It is currently very alpha and misses a lot of functionality.

ofxGCodePlotter will _never_ be a GCode library. I personally only use it for converting ofPaths in 2D into one layer GCode.

## Usage
* Install the addon like any other openFrameworks addon. ofxGCodePlotter uses ofxCubicBezierToBiarc to convert cubic bezier curves to arcs - so that addon will also be pulled in.

* Next copy the example config file and adjust the settings as needed.

* Then create an instance of ofxGCodePlotter like so: `ofxGCodePlotter gcode = ofxGCodePlotter('path/to/config', width, height);`.

* Feed it with a vector of ofPaths: `vector<GCodePath> paths = gcode.generate(shapes);`

* Print to the console: ``gcode.print(paths);

## Limitations / Roadmap
* Shortest path
* Print filled paths (hatching).
* Cut paths under a filled path.
* Do not print off canvas content.
* Inject a tool.
* Feed rate.

## Liability
If you use this addon with your CNC machine things may break (including your machine). That is not my responsibility. See LICENSE.txt.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
