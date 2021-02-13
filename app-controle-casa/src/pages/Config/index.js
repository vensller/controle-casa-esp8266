import React, { useState } from 'react';
import {Alert, ActivityIndicator} from 'react-native';
import { useSelector, useDispatch } from 'react-redux';
import Icon from 'react-native-vector-icons/MaterialIcons';
import {withNavigationFocus} from 'react-navigation';

import Background from '~/Components/Background';
import Input from '~/Components/Input';
import Button from '~/Components/Button';

import {Container, Title, RestartButton, Text, LogsContainer, ReloadButton} from './styles';

import { updateControllerIp } from '~/store/modules/user/actions';
import api from '~/services/api';
import { TouchableOpacity } from 'react-native-gesture-handler';

function Configurations() {  
  const dispatch = useDispatch();
  const socketIp = useSelector(state => state.user.controllerIP);
  const [ip, setIp] = useState('');
  const [logs, setLogs] = useState('');
  const [loading, setLoading] = useState(true);
  const [reload, setReload] = useState(false);

  function handleChange() {
    dispatch(updateControllerIp(ip));
    Alert.alert('Sucesso', 'Endereço alterado');
  }

  async function handleRestart() {
    try {   
      await api.post(socketIp, 'reiniciar');
      Alert.alert('OK', "Comando enviado");
    } catch (error) {
      Alert.alert('Erro', error.message);
    }
  }
 
  React.useEffect(() => {
    async function loadLogs() {
      setLoading(true);
      try {        
        const response = await api.post(socketIp, 'mostrarLogs');
        setLogs(response.data);
      } catch (error) {    
        Alert.alert('Erro', error.message);
      }
      setLoading(false);
      setReload(false);
    }

    if (socketIp && !ip) {
      setIp(socketIp);

      loadLogs();
    }    

    if (reload) {
      loadLogs();
    }
  }, [socketIp, reload]);

  return (
    <Background>
      <Container>
        <Title>Configurações</Title>
        <Input icon="network-wifi" value={ip} onChangeText={setIp} placeholder="Endereço do microcontrolador"/>
        <Button onPress={handleChange}>Salvar</Button>
        <RestartButton onPress={handleRestart}>
          <Text>Reiniciar ESP8266</Text>
        </RestartButton>
        <LogsContainer>
          {loading ? (
            <ActivityIndicator color="#fff" size={36}/>            
          ) : (
            <>
              <ReloadButton onPress={() => setReload(true)}>
                <Icon name="cached" size={20} color="#fff" />
              </ReloadButton>
              <Text>{logs}</Text>
            </>            
          )}                  
        </LogsContainer>
      </Container>
    </Background>
  );
}

Configurations.navigationOptions = {
  tabBarLabel: 'Configurações',
  tabBarIcon: ({tintColor}) => (
    <Icon name="settings" size={20} color={tintColor} />
  ),
};

export default withNavigationFocus(Configurations);
