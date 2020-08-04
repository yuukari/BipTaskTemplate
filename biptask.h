/***************************************************/
/*                                                 */
/*     BipTask data transfer library for BipOS     */
/*               Developed by Yuukari              */
/*                                                 */
/*               Library version: 1.0              */
/*       Сompatible with BipTask app v1.1.0        */
/*                                                 */
/*    https://github.com/Yuukari/BipTaskLibrary    */
/*                                                 */
/***************************************************/

/*
    biptask_send_byte - Отправляет один байт данных

    @param char* app_id - массив char размером в три элемента, содержащий уникальный
    идентификатор приложения
    @param char byte - отправляемое значение
*/
void biptask_send_byte(char* app_id, char byte);

/*
    biptask_send_byte - Отправляет массив байт данных

    @param char* app_id - массив char размером в три элемента, содержащий уникальный
    идентификатор приложения
    @param char* bytes - массив с данными
    @param unsigned int length - размер массива bytes
*/
void biptask_send_bytes(char* app_id, char* bytes, unsigned int length);