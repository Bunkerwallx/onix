

use strict;
use warnings;
use Telegram::Bot::Authenticate;
use LWP::UserAgent;

# Configurar los datos de autenticación del bot de Telegram
my $token = 'TOKEN_DEL_BOT_DE_TELEGRAM';

# Crear una instancia del objeto de autenticación
my $auth = Telegram::Bot::Authenticate->new(token => $token);

# Obtener la dirección y el User-Agent desde la entrada del usuario
print "Ingrese la dirección IP: ";
my $ip_address = <STDIN>;
chomp $ip_address;

print "Ingrese el User-Agent de iOS: ";
my $user_agent = <STDIN>;
chomp $user_agent;

# Configurar el objeto UserAgent con la dirección IP y el User-Agent ingresados
my $ua = LWP::UserAgent->new;
$ua->proxy(['http', 'https'], "http://$ip_address");
$ua->agent($user_agent);

# Validar un usuario de Telegram
my $user_id = 123456789; # ID del usuario a validar
my $is_valid_user = $auth->validate_user($user_id, $ua);

if ($is_valid_user) {
    print "El usuario $user_id es válido.\n";
} else {
    print "El usuario $user_id no es válido.\n";
}

# Validar un grupo de Telegram
my $group_id = -123456789; # ID del grupo a validar
my $is_valid_group = $auth->validate_group($group_id, $ua);

if ($is_valid_group) {
    print "El grupo $group_id es válido.\n";
} else {
    print "El grupo $group_id no es válido.\n";
}

