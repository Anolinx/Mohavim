# Maintainer: Gabriel A. Matos <gabriellramone@gmail.com>

pkgname=mohavim
pkgver=7.3
pkgrel=1
pkgdesc="Editor de texto moderno com arquitetura modular"
arch=('x86_64' 'i686' 'aarch64')
url="https://"
license=('GPL3')
depends=('glibc')
makedepends=('gcc' 'make')
source=("$pkgname-$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$srcdir/$pkgname-$pkgver/src"
    make clean
    make
}

package() {
    cd "$srcdir/$pkgname-$pkgver"
    
    # Instalar o executável
    install -Dm755 src/mohavim "$pkgdir/usr/bin/mohavim"
    
    # Instalar documentação
    install -Dm644 README.md "$pkgdir/usr/share/doc/$pkgname/README.md"
    install -Dm644 CODIGO_FONTE.md "$pkgdir/usr/share/doc/$pkgname/CODIGO_FONTE.md"
    
    # Instalar licença
    install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
