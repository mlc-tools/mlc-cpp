set -e

function increment_version(){
	version=$(grep 'APP_VERSION' mlc-cpp/version.hpp | sed -E 's/.*"([^"]+)".*/\1/')
	IFS='.' read -r major minor patch <<< "$version"
	patch=$((patch + 1))
	new_version="$major.$minor.$patch"
	if [[ "$OSTYPE" == "darwin"* ]]; then
	  sed -i '' -E "s/(APP_VERSION = \").*(\";)/\1$new_version\2/" mlc-cpp/version.hpp
	else
	  sed -i -E "s/(APP_VERSION = \").*(\";)/\1$new_version\2/" mlc-cpp/version.hpp
	fi
	echo $new_version
}

function build(){
	mkdir -p build_cli
	cd build_cli
	cmake -DCMAKE_BUILD_TYPE=Release ..
	cmake --build .  -j8
	cmake --install . --prefix "$HOME/.local"
}

function push_tag(){
	TAG="v/$1"
	git tag $TAG
	git push origin --tags
}

pushd .
cd ../
VERSION=$(increment_version)
build
push_tag $VERSION
echo "New version is: " "$VERSION"

popd
