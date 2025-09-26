pushd . > /dev/null

set -e

function increment_version(){
	version=$(grep 'APP_VERSION' ../mlc-cpp/version.hpp | sed -E 's/.*"([^"]+)".*/\1/')
	# echo "Текущая версия: $version"

	# 2. Разбираем версию на части
	IFS='.' read -r major minor patch <<< "$version"

	# 3. Увеличиваем "patch" (последний номер)
	patch=$((patch + 1))

	# 4. Собираем новую версию
	new_version="$major.$minor.$patch"

	# echo "Новая версия: $new_version"

	# 5. Записываем в файл (если нужно переписать version.hpp)
	# sed -i работает по-разному на Linux и macOS, поэтому:
	if [[ "$OSTYPE" == "darwin"* ]]; then
	  sed -i '' -E "s/(APP_VERSION = \").*(\";)/\1$new_version\2/" ../mlc-cpp/version.hpp
	else
	  sed -i -E "s/(APP_VERSION = \").*(\";)/\1$new_version\2/" ../mlc-cpp/version.hpp
	fi
	
	echo "$new_version"
}

function build(){
	mkdir -p ../build_cli
	cd ../build_cli
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make -j8
	cmake --install . --prefix "$HOME/.local"
}

# $1 - version
function upload(){
	git clone git@github.com:mlc-tools/mlc-binary-app-versions.git bin --depth 1 | echo 'repository exist'
	cp mlc bin/

	cd bin
	git checkout -b versions | echo 'branch exist'
	MESSAGE="osx/$1"
	git add mlc
	git commit -m "added binary $MESSAGE"
	git tag $MESSAGE
	git push --all -f
	git push origin versions
	git push origin --tags
}

VERSION=$(increment_version)
echo $VERSION
build
upload $VERSION

popd > /dev/null