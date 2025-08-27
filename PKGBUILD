# Maintainer: Gabriel A. Matos <gabriellramone@gmail.com>

pkgname=mohavim
pkgver=7.2
pkgrel=1
pkgdesc="Editor de texto modernizado
arch=('x86_64' 'i686' 'aarch64')
url="https://"
license=('GPL3')
depends=('glibc')
makedepends=('gcc' 'make')
source=("$pkgname-$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$srcdir/$pkgname-$pkgver/MohavimEditor"
    gcc -Wall -O2 -o mohavim mohavim.c
}

package() {
    cd "$srcdir/$pkgname-$pkgver/MohavimEditor"
    
    # Instalar o executável
    install -Dm755 mohavim "$pkgdir/usr/bin/mohavim"
    
    # Instalar documentação
    install -Dm644 ../README.md "$pkgdir/usr/share/doc/$pkgname/README.md"
    
}
