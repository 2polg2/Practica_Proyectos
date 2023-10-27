#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_NAME_LENGTH 30
#define MAX_CHANNEL_NAME_LENGTH 50

typedef struct
{
    int id;
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char filename[MAX_NAME_LENGTH];
} User;

typedef struct
{
    int id;
    char name[MAX_CHANNEL_NAME_LENGTH];
    int budget;
} Channel;

int nextUserId = 0;
int nextChannelId = 0;

void CreateCanal()
{
    Channel *channel = (Channel *)malloc(sizeof(Channel));

    if (channel == NULL)
    {
        printf("Error: could not allocate memory for channel.\n");
        return;
    }

    printf("Por favor introduzca el nombre del canal: ");
    fgets(channel->name, MAX_CHANNEL_NAME_LENGTH, stdin);
    channel->name[strlen(channel->name) - 1] = '\0';

    printf("Por favor introduzca el presupuesto del canal: ");
    scanf("%d", &channel->budget);

    FILE *file = fopen("channels.txt", "a");

    if (file == NULL)
    {
        printf("Error: could not open file.\n");
        return;
    }

    channel->id = ++nextChannelId;
    fprintf(file, "%04d, %s, %d\n", channel->id, channel->name, channel->budget);
    fclose(file);

    printf("\nCanal creado correctamente!\n");
    printf("ID: %04d\n", channel->id);
    printf("Nombre: %s\n", channel->name);
    printf("Presupuesto: %d\n", channel->budget);

    free(channel);
}
int passwordHash(User *user)
{
    int i = 0;
    char aux;
    while ((aux = getch()) != '\n' && aux != '\r')
    {
        user->password[i] = aux;
        if (aux == '\b' && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else
        {
            printf("*");
            i++;
        }
    }
    user->password[i] = '\0';
}

// Función para guardar la información del usuario en un archivo de texto
void saveUserToFile(User *user)
{

    FILE *file = fopen(user->filename, "a");

    if (file == NULL)
    {
        printf("Error: could not open file.\n");
        return;
    }

    fprintf(file, "%04d, %s, %s, %s\n", user->id, user->name, user->email, user->password);
    fclose(file);
}

// Función para verificar si el usuario ya existe en el archivo de texto
void checkUser(User *user)
{
    int id = 0;
    char fila[100];
    char name[MAX_NAME_LENGTH];
    char fullname[MAX_NAME_LENGTH];
    char mail[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int count = 0;
    int i = 0;

    FILE *file = fopen(user->filename, "r");

    while (!feof(file) && i - 1 != user->id)
    {
        strcpy(fullname, "");
        fscanf(file, "%d%*c%*c", &id);
        fscanf(file, "%s", name);

        while (name[strlen(name) - 1] != ',' && !feof(file))
        {
            strcat(fullname, name);
            strcat(fullname, " ");
            fscanf(file, "%s", name);
            count++;
        }

        if (count > 1)
        {
            strcat(fullname, name);
            fullname[strlen(fullname) - 1] = '\0';
        }

        fscanf(file, "%s%*c", &mail);
        mail[strlen(mail) - 1] = '\0';

        fscanf(file, "%s", &password);
        password[strlen(password)] = '\0';

        printf("\n%d\n", id);
        printf("%s\n", fullname);
        printf("%s\n", mail);
        printf("%s\n\n", password);
        fgets(fila, 100, file);
        i++;
    }

    if (strcmp(mail, user->email) == 0)
    {
        user->id = -1;
        printf("\nEl usuario ya existe.\n");
        if (strcmp(password, user->password) == 0)
        {
            user->id = -2;
        }
        fclose(file);
    }

    else
    {
        id++;
        user->id = id;
    }
}

// Función para crear un nuevo usuario y solicitar su información al usuario
User *createUser(User *user)
{
    char password2[MAX_PASSWORD_LENGTH];

    if (user == NULL)
    {
        printf("Error: could not allocate memory for user.\n");
        return NULL;
    }

    printf("Por favor introduzca su nombre completo: ");
    fgets(user->name, MAX_NAME_LENGTH, stdin);
    user->name[strlen(user->name) - 1] = '\0';

    printf("Por favor introduzca su email: ");
    scanf("%s", user->email);

    printf("Por favor introduzca su password: ");
    passwordHash(user);
    strcpy(password2, user->password);
    printf("\nPor favor introduzca su password nuevamente: ");
    passwordHash(user);

    while (strcmp(user->password, password2) != 0)
    {
        printf("\nLas passwords no coinciden, por favor introduzca su password nuevamente: ");
        passwordHash(user);
    }

    checkUser(user);

    if (user->id != -1)
    {
        saveUserToFile(user);
    }
    else
        printf("\nError: Ya se a usado este mail.\n");
    return user;
}

void loginUser(User *user)
{
    int opcion = 0, comando = 0;
    printf("Quieres log-in como 1. cliente o 2. productor?\n");
    scanf("%d%*c", &opcion);

    while (1 > opcion && 2 < opcion)
    {
        printf("Opcion no valida\n");
        printf("Quieres log-in como 1. cliente o 2. productor?\n");
        scanf("%d%*c", &opcion);
    }

    switch (opcion)
    {
    case 1:
        strcpy(user->filename, "clientes.txt");
        break;
    case 2:
        strcpy(user->filename, "productores.txt");
        break;
    }

    printf("mail: ");
    scanf("%s", user->email);
    printf("password: ");
    passwordHash(user);
    checkUser(user);

    if (user->id == -2)
    {
        printf("\nLOGGED IN.\n");

        if (opcion == 2)
        {

            while (comando != 4)
            {
                printf("Vienvenido a la plataforma de los productores\n");
                printf("1. Crear canal de TV\n");
                printf("2. Gestionar clientes\n");
                printf("3. Proximamente\n");
                printf("4. Salir\n");

                printf("Que desa hacer hacer: ");
                scanf("%d", &comando);

                if (comando == 1)
                {
                    CreateCanal();
                }

                if (commando == 2)
                {
                    showUsers();
                }
                else
                {
                    printf("Estanmos en desarollo para nuevas acciones");
                }
            }
        }
    }
    else if (user->id == -1)
    {
        printf("\nError: Incorrect password.\n");
    }
    else
    {
        printf("\nMail not founded\n");
    }
}

void showUsers()
{
    int option = 0;
    char line[100];
    char(*a[100])[100];
    int i = 0;
    int id = 0;
    int search_id = 0;
    int count = 0;

    FILE *file2 = fopen("clientes.txt", "r");
    fgets(line, 100, file2);
    while (feof(file2) == 0)
    {
        a[i] = (char(*)[100])malloc(sizeof(char) * 100);
        a[i] = strcpy(a[i], line);
        printf("%s", a[i]);
        fgets(line, 100, file2);
        i++;
    }

    printf("1. delete user\n");
    printf("2. update user\n");
    printf("3. exit\n");
    scanf("%d", &option);

    fclose(file2);

    switch (option)
    {
    case 1:
        FILE *file2 = fopen("clientes.txt", "r");
        printf("Por favor introduzca el id del usuario que desea borrar: ");
        scanf("%d", &id);

        while (feof(file2) == 0)
        {
            fscanf(file2, "%d", &search_id);
            if (id == search_id)
            {
                fclose(file2);
                FILE *file2 = fopen("clientes.txt", "w");

                for (int j = 0; j < i; j++)
                {
                    if (count != j)
                    {
                        fprintf(file2, "%s", a[j]);
                    }
                }
                break;
            }
            fgets(line, 100, file2);
            count++;
        }
        break;

    default:
        break;
    }
}

// Función para mostrar un menú de opciones al usuario y devolver la opción seleccionada
int menu(int opcion)
{

    printf(" 1. Iniciar sesion\n");
    printf(" 2. Registrarse\n");
    printf(" 3. Salir\n");
    printf("Acceder: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > 3)
    {
        printf("\nOpción no valida\n");
        opcion = menu(opcion);
    }
    return opcion;
}

int main()
{
    /*Leer el ID del siguiente usuario desde un archivo
    FILE *file = fopen("nextUserId.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &nextUserId);
        fclose(file);
    }*/
    User *user = (User *)malloc(sizeof(User));
    int contraseña_privada = 1234;
    int contraseña_unic = 0;

    int opcion = 0,
        opcion2 = 0;
    char aux = ' ';

    while (opcion != 3)
    {
        opcion = menu(opcion);

        switch (opcion)
        {

        case 1:

            loginUser(user);
            break;

        case 2:

            // Solicitar al usuario que seleccione su tipo de usuario
            printf("Quieres registrarte como 1.cliente o 2.productor?\n");
            scanf("%d%*c", &opcion2);

            while (opcion2 < 1 || opcion2 > 2)
            {
                printf("Opcion no valida\n");
                printf("Quieres registrarte como 1.cliente o 2.productor?\n");
                scanf("%d%*c", &opcion2);
            }

            // Crear un nuevo usuario y mostrar su información
            if (opcion2 == 1)
            {
                strcpy(user->filename, "clientes.txt");
                createUser(user);
                if (user->id != -1)
                {

                    printf("\nUsuario creado correctamente!\n");
                    printf("ID: %04d\n", user->id);
                    printf("Nombre: %s\n", user->name);
                    printf("Email: %s\n", user->email);
                    printf("Password: %s\n", user->password);
                    free(user);
                }
            }

            else if (opcion2 == 2)
            {
                strcpy(user->filename, "productores.txt");
                createUser(user);

                if (user->id > 0)
                {
                    printf("\nUsuario creado correctamente!\n");
                    printf("ID: %04d\n", user->id);
                    printf("Nombre: %s\n", user->name);
                    printf("Email: %s\n", user->email);
                    printf("Password: %s\n", user->password);
                    free(user);
                }
            }

            break;

        case 3:

            break;
        }

        /* Guardar el ID del siguiente usuario en un archivo
        file = fopen("nextUserId.txt", "w");
        if (file != NULL)
        {
            fprintf(file, "%d", nextUserId);
            fclose(file);
        }*/
    }
    return 0;
}